#ifndef VSTRIKE_NETWORK_MANAGER_H
#define VSTRIKE_NETWORK_MANAGER_H

#include "UDPSocket.h"
#include "NetworkProtocol.h"
#include <string>
#include <functional>

// ============================================================================
// Network Role and Connection States
// ============================================================================
enum class NetworkRole
{
    NONE,
    HOST,  // Player 1: Runs authoritative physics, listens on port
    CLIENT // Player 2: Connects to Host IP, sends paddle inputs
};

enum class ConnectionState
{
    DISCONNECTED,
    WAITING_FOR_PEER, // Host waiting for Client, or Client attempting to join
    CONNECTED         // Actively exchanging gameplay packets
};

// ============================================================================
// NetworkManager: Orchestrates multiplayer session, packet dispatch & telemetry
// ============================================================================
class NetworkManager
{
private:
    UDPSocket socket;
    NetworkRole role = NetworkRole::NONE;
    ConnectionState state = ConnectionState::DISCONNECTED;

    std::string remoteIp = "127.0.0.1";
    uint16_t remotePort = DEFAULT_SERVER_PORT;
    uint16_t localPort = DEFAULT_SERVER_PORT;

    uint32_t localSequence = 0;
    uint32_t lastReceivedSequence = 0;

    // Telemetry & Latency (RTT)
    float pingTimer = 0.0f;
    int currentPingMs = 0;
    float timeSinceLastPacket = 0.0f;

    // Latest replicated game state (for Client)
    PacketServerState latestServerState{};
    bool newServerStateAvailable = false;

    // Latest received client input (for Host)
    PacketClientInput latestClientInput{};
    bool newClientInputAvailable = false;

    // Tick timers
    float networkTickTimer = 0.0f;
    const float NETWORK_TICK_RATE = 1.0f / 60.0f; // 60 updates per second

    void ProcessIncomingPacket(const uint8_t* data, size_t size, const std::string& senderIp, uint16_t senderPort);

public:
    NetworkManager();
    ~NetworkManager();

    // Start as Host (Server) listening on specified port
    bool StartHost(uint16_t port = DEFAULT_SERVER_PORT);

    // Start as Client connecting to Host IP
    bool StartClient(const std::string& hostIp, uint16_t port = DEFAULT_SERVER_PORT);

    // Called every frame inside the game loop to drain packets & maintain connection
    void Update(float dt);

    // Host API: Broadcast authoritative game world state to client
    void SendServerState(float bX, float bY, float bVx, float bVy, float p1Y, float p2Y, float p1Hp, float p2Hp);

    // Client API: Send local paddle position & velocity to host
    void SendClientInput(float paddleY, float paddleVelocityY);

    // Check if new data has arrived
    bool ConsumeServerState(PacketServerState& outState);
    bool ConsumeClientInput(PacketClientInput& outInput);

    // Disconnect
    void Disconnect();

    // Connection and telemetry getters
    NetworkRole GetRole() const { return role; }
    ConnectionState GetState() const { return state; }
    int GetPingMs() const { return currentPingMs; }
    bool IsConnected() const { return state == ConnectionState::CONNECTED; }
};

#endif // VSTRIKE_NETWORK_MANAGER_H
