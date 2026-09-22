#define ENET_IPV4_ONLY
#include "ENetManager.h"
#include "enet.h"
#include <iostream>

static bool isENetInitialized = false;

ENetManager::ENetManager()
{
    if (!isENetInitialized && enet_initialize() == 0)
    {
        isENetInitialized = true;
    }
}

ENetManager::~ENetManager()
{
    Disconnect();
}

bool ENetManager::StartHost(uint16_t port)
{
    Disconnect();
    role = NetworkRole::HOST;

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(&address, 2, ENET_CHANNEL_COUNT, 0, 0);
    if (!host)
    {
        role = NetworkRole::NONE;
        return false;
    }

    state = ConnectionState::WAITING_FOR_PEER;
    return true;
}

bool ENetManager::StartClient(const std::string& hostIp, uint16_t port)
{
    Disconnect();
    role = NetworkRole::CLIENT;

    host = enet_host_create(nullptr, 1, ENET_CHANNEL_COUNT, 0, 0);
    if (!host)
    {
        role = NetworkRole::NONE;
        return false;
    }

    ENetAddress address;
    enet_address_set_host(&address, hostIp.c_str());
    address.port = port;

    peer = enet_host_connect(host, &address, ENET_CHANNEL_COUNT, 0);
    if (!peer)
    {
        enet_host_destroy(host);
        host = nullptr;
        role = NetworkRole::NONE;
        return false;
    }

    state = ConnectionState::WAITING_FOR_PEER;
    return true;
}

void ENetManager::Disconnect()
{
    if (peer)
    {
        enet_peer_disconnect(peer, 0);
        ENetEvent event;
        while (enet_host_service(host, &event, 20) > 0)
        {
            if (event.type == ENET_EVENT_TYPE_RECEIVE) enet_packet_destroy(event.packet);
            else if (event.type == ENET_EVENT_TYPE_DISCONNECT) break;
        }
        peer = nullptr;
    }

    if (host)
    {
        enet_host_destroy(host);
        host = nullptr;
    }

    role = NetworkRole::NONE;
    state = ConnectionState::DISCONNECTED;
    newServerStateAvailable = false;
    newClientInputAvailable = false;
}

void ENetManager::Update(float dt)
{
    if (!host) return;

    ENetEvent event;
    while (enet_host_service(host, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            peer = event.peer;
            state = ConnectionState::CONNECTED;
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            HandlePacket(event.packet->data, event.packet->dataLength, event.channelID);
            enet_packet_destroy(event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            state = ConnectionState::DISCONNECTED;
            peer = nullptr;
            break;

        default:
            break;
        }
    }
}

void ENetManager::HandlePacket(const uint8_t* data, size_t size, uint8_t channelID)
{
    if (size < sizeof(PacketHeader)) return;

    const PacketHeader* header = reinterpret_cast<const PacketHeader*>(data);
    if (header->magic != VSTRIKE_NET_MAGIC) return;

    if (header->type == PacketType::CLIENT_INPUT && role == NetworkRole::HOST && size >= sizeof(PacketClientInput))
    {
        latestClientInput = *reinterpret_cast<const PacketClientInput*>(data);
        newClientInputAvailable = true;
    }
    else if (header->type == PacketType::SERVER_STATE && role == NetworkRole::CLIENT && size >= sizeof(PacketServerState))
    {
        const PacketServerState* statePkt = reinterpret_cast<const PacketServerState*>(data);
        if (statePkt->header.sequenceNumber > lastReceivedSequence)
        {
            lastReceivedSequence = statePkt->header.sequenceNumber;
            latestServerState = *statePkt;
            newServerStateAvailable = true;
        }
    }
}

void ENetManager::SendServerState(float bX, float bY, float bVx, float bVy, float p1Y, float p2Y, float p1Hp, float p2Hp)
{
    if (role != NetworkRole::HOST || state != ConnectionState::CONNECTED || !peer) return;

    PacketServerState pkt{};
    pkt.header.magic = VSTRIKE_NET_MAGIC;
    pkt.header.type = PacketType::SERVER_STATE;
    pkt.header.sequenceNumber = ++localSequence;
    pkt.ballX = bX; pkt.ballY = bY;
    pkt.ballVx = bVx; pkt.ballVy = bVy;
    pkt.paddle1Y = p1Y; pkt.paddle2Y = p2Y;
    pkt.paddle1Hp = p1Hp; pkt.paddle2Hp = p2Hp;
    pkt.serverTick = localSequence;

    ENetPacket* packet = enet_packet_create(&pkt, sizeof(pkt), 0);
    enet_peer_send(peer, CHANNEL_UNRELIABLE_STATE, packet);
}

void ENetManager::SendClientInput(float paddleY, float paddleVelocityY)
{
    if (role != NetworkRole::CLIENT || state != ConnectionState::CONNECTED || !peer) return;

    PacketClientInput pkt{};
    pkt.header.magic = VSTRIKE_NET_MAGIC;
    pkt.header.type = PacketType::CLIENT_INPUT;
    pkt.header.sequenceNumber = ++localSequence;
    pkt.paddleY = paddleY;
    pkt.paddleVelocityY = paddleVelocityY;

    ENetPacket* packet = enet_packet_create(&pkt, sizeof(pkt), 0);
    enet_peer_send(peer, CHANNEL_UNRELIABLE_STATE, packet);
}

bool ENetManager::ConsumeServerState(PacketServerState& outState)
{
    if (!newServerStateAvailable) return false;
    outState = latestServerState;
    newServerStateAvailable = false;
    return true;
}

bool ENetManager::ConsumeClientInput(PacketClientInput& outInput)
{
    if (!newClientInputAvailable) return false;
    outInput = latestClientInput;
    newClientInputAvailable = false;
    return true;
}

int ENetManager::GetPingMs() const
{
    return (peer && state == ConnectionState::CONNECTED) ? static_cast<int>(peer->roundTripTime) : 0;
}
