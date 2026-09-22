#include "NetworkManager.h"
#include <iostream>
#include <chrono>

// Helper to get system time in milliseconds
static uint32_t GetCurrentTimeMs()
{
    using namespace std::chrono;
    return static_cast<uint32_t>(duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()).count());
}

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
    Disconnect();
}

bool NetworkManager::StartHost(uint16_t port)
{
    Disconnect();
    localPort = port;
    role = NetworkRole::HOST;

    if (!socket.Open(localPort))
    {
        std::cerr << "[NetworkManager] Failed to start host on port " << localPort << std::endl;
        role = NetworkRole::NONE;
        return false;
    }

    state = ConnectionState::WAITING_FOR_PEER;
    std::cout << "[NetworkManager] Host started. Waiting for opponent on port " << localPort << "..." << std::endl;
    return true;
}

bool NetworkManager::StartClient(const std::string& hostIp, uint16_t port)
{
    Disconnect();
    remoteIp = hostIp;
    remotePort = port;
    role = NetworkRole::CLIENT;

    // Client opens on port 0 -> OS assigns an arbitrary free ephemeral port (e.g. 54321)
    if (!socket.Open(0))
    {
        std::cerr << "[NetworkManager] Failed to create client socket" << std::endl;
        role = NetworkRole::NONE;
        return false;
    }

    state = ConnectionState::WAITING_FOR_PEER;
    std::cout << "[NetworkManager] Client started. Connecting to " << remoteIp << ":" << remotePort << "..." << std::endl;

    // Send initial handshake request
    PacketHandshake req{};
    req.header.magic = VSTRIKE_NET_MAGIC;
    req.header.type = PacketType::HANDSHAKE_REQUEST;
    req.header.sequenceNumber = ++localSequence;
    req.assignedPlayerId = 2;

    socket.SendTo(remoteIp, remotePort, &req, sizeof(req));
    return true;
}

void NetworkManager::Disconnect()
{
    if (state == ConnectionState::CONNECTED)
    {
        PacketHeader disc{};
        disc.magic = VSTRIKE_NET_MAGIC;
        disc.type = PacketType::DISCONNECT;
        disc.sequenceNumber = ++localSequence;
        socket.SendTo(remoteIp, remotePort, &disc, sizeof(disc));
    }

    socket.Close();
    role = NetworkRole::NONE;
    state = ConnectionState::DISCONNECTED;
    newServerStateAvailable = false;
    newClientInputAvailable = false;
}

void NetworkManager::Update(float dt)
{
    if (role == NetworkRole::NONE || !socket.IsOpen()) return;

    timeSinceLastPacket += dt;

    // 1. Drain all incoming packets from the non-blocking UDP socket
    uint8_t buffer[1024];
    std::string senderIp;
    uint16_t senderPort = 0;
    int bytesRead = 0;

    // We process ALL queued packets in a single frame so there is zero input lag!
    while ((bytesRead = socket.RecvFrom(buffer, sizeof(buffer), senderIp, senderPort)) > 0)
    {
        ProcessIncomingPacket(buffer, static_cast<size_t>(bytesRead), senderIp, senderPort);
    }

    // 2. Client Handshake Retry Logic (if still waiting for host to respond)
    if (role == NetworkRole::CLIENT && state == ConnectionState::WAITING_FOR_PEER)
    {
        pingTimer += dt;
        if (pingTimer >= 0.5f) // Retry handshake every 500ms
        {
            pingTimer = 0.0f;
            PacketHandshake req{};
            req.header.magic = VSTRIKE_NET_MAGIC;
            req.header.type = PacketType::HANDSHAKE_REQUEST;
            req.header.sequenceNumber = ++localSequence;
            req.assignedPlayerId = 2;
            socket.SendTo(remoteIp, remotePort, &req, sizeof(req));
        }
    }

    // 3. Heartbeat / Ping for RTT latency tracking (every 1 second when connected)
    if (state == ConnectionState::CONNECTED)
    {
        pingTimer += dt;
        if (pingTimer >= 1.0f)
        {
            pingTimer = 0.0f;
            PacketPingPong ping{};
            ping.header.magic = VSTRIKE_NET_MAGIC;
            ping.header.type = PacketType::PING;
            ping.header.sequenceNumber = ++localSequence;
            ping.clientTimestampMs = GetCurrentTimeMs();
            socket.SendTo(remoteIp, remotePort, &ping, sizeof(ping));
        }
    }
}

void NetworkManager::ProcessIncomingPacket(const uint8_t* data, size_t size, const std::string& senderIp, uint16_t senderPort)
{
    if (size < sizeof(PacketHeader)) return;

    const PacketHeader* header = reinterpret_cast<const PacketHeader*>(data);
    if (header->magic != VSTRIKE_NET_MAGIC) return; // Drop unrecognized garbage

    timeSinceLastPacket = 0.0f;

    switch (header->type)
    {
    case PacketType::HANDSHAKE_REQUEST:
        if (role == NetworkRole::HOST)
        {
            remoteIp = senderIp;
            remotePort = senderPort;
            state = ConnectionState::CONNECTED;

            // Send accept response back to client
            PacketHandshake ack{};
            ack.header.magic = VSTRIKE_NET_MAGIC;
            ack.header.type = PacketType::HANDSHAKE_ACCEPT;
            ack.header.sequenceNumber = ++localSequence;
            ack.assignedPlayerId = 2;
            socket.SendTo(remoteIp, remotePort, &ack, sizeof(ack));

            std::cout << "[NetworkManager] Opponent connected from " << remoteIp << ":" << remotePort << "!" << std::endl;
        }
        break;

    case PacketType::HANDSHAKE_ACCEPT:
        if (role == NetworkRole::CLIENT && state == ConnectionState::WAITING_FOR_PEER)
        {
            state = ConnectionState::CONNECTED;
            std::cout << "[NetworkManager] Connected to Host at " << remoteIp << ":" << remotePort << "!" << std::endl;
        }
        break;

    case PacketType::CLIENT_INPUT:
        if (role == NetworkRole::HOST && size >= sizeof(PacketClientInput))
        {
            const PacketClientInput* input = reinterpret_cast<const PacketClientInput*>(data);
            latestClientInput = *input;
            newClientInputAvailable = true;
        }
        break;

    case PacketType::SERVER_STATE:
        if (role == NetworkRole::CLIENT && size >= sizeof(PacketServerState))
        {
            const PacketServerState* statePkt = reinterpret_cast<const PacketServerState*>(data);
            // Drop out-of-order packets: only accept if newer than our last processed packet
            if (statePkt->header.sequenceNumber > lastReceivedSequence)
            {
                lastReceivedSequence = statePkt->header.sequenceNumber;
                latestServerState = *statePkt;
                newServerStateAvailable = true;
            }
        }
        break;

    case PacketType::PING:
        {
            // Echo back as PONG with the same original timestamp
            if (size >= sizeof(PacketPingPong))
            {
                const PacketPingPong* req = reinterpret_cast<const PacketPingPong*>(data);
                PacketPingPong pong = *req;
                pong.header.type = PacketType::PONG;
                pong.header.sequenceNumber = ++localSequence;
                socket.SendTo(senderIp, senderPort, &pong, sizeof(pong));
            }
        }
        break;

    case PacketType::PONG:
        if (size >= sizeof(PacketPingPong))
        {
            const PacketPingPong* pong = reinterpret_cast<const PacketPingPong*>(data);
            uint32_t now = GetCurrentTimeMs();
            if (now >= pong->clientTimestampMs)
            {
                currentPingMs = static_cast<int>(now - pong->clientTimestampMs);
            }
        }
        break;

    case PacketType::DISCONNECT:
        std::cout << "[NetworkManager] Peer disconnected." << std::endl;
        state = ConnectionState::DISCONNECTED;
        break;
    }
}

void NetworkManager::SendServerState(float bX, float bY, float bVx, float bVy, float p1Y, float p2Y, float p1Hp, float p2Hp)
{
    if (role != NetworkRole::HOST || state != ConnectionState::CONNECTED) return;

    PacketServerState pkt{};
    pkt.header.magic = VSTRIKE_NET_MAGIC;
    pkt.header.type = PacketType::SERVER_STATE;
    pkt.header.sequenceNumber = ++localSequence;

    pkt.ballX = bX;
    pkt.ballY = bY;
    pkt.ballVx = bVx;
    pkt.ballVy = bVy;
    pkt.paddle1Y = p1Y;
    pkt.paddle2Y = p2Y;
    pkt.paddle1Hp = p1Hp;
    pkt.paddle2Hp = p2Hp;
    pkt.serverTick = localSequence;

    socket.SendTo(remoteIp, remotePort, &pkt, sizeof(pkt));
}

void NetworkManager::SendClientInput(float paddleY, float paddleVelocityY)
{
    if (role != NetworkRole::CLIENT || state != ConnectionState::CONNECTED) return;

    PacketClientInput pkt{};
    pkt.header.magic = VSTRIKE_NET_MAGIC;
    pkt.header.type = PacketType::CLIENT_INPUT;
    pkt.header.sequenceNumber = ++localSequence;

    pkt.paddleY = paddleY;
    pkt.paddleVelocityY = paddleVelocityY;
    pkt.clientTimestamp = GetCurrentTimeMs();

    socket.SendTo(remoteIp, remotePort, &pkt, sizeof(pkt));
}

bool NetworkManager::ConsumeServerState(PacketServerState& outState)
{
    if (!newServerStateAvailable) return false;
    outState = latestServerState;
    newServerStateAvailable = false;
    return true;
}

bool NetworkManager::ConsumeClientInput(PacketClientInput& outInput)
{
    if (!newClientInputAvailable) return false;
    outInput = latestClientInput;
    newClientInputAvailable = false;
    return true;
}
