#ifndef VSTRIKE_ENET_MANAGER_H
#define VSTRIKE_ENET_MANAGER_H

#include "NetworkProtocol.h"
#include <string>
#include <cstdint>

struct _ENetHost;
typedef struct _ENetHost ENetHost;
struct _ENetPeer;
typedef struct _ENetPeer ENetPeer;

enum class NetworkRole { NONE, HOST, CLIENT };
enum class ConnectionState { DISCONNECTED, WAITING_FOR_PEER, CONNECTED };

constexpr uint8_t CHANNEL_UNRELIABLE_STATE = 0;
constexpr uint8_t CHANNEL_RELIABLE_EVENTS   = 1;
constexpr size_t  ENET_CHANNEL_COUNT        = 2;

class ENetManager
{
private:
    ENetHost* host = nullptr;
    ENetPeer* peer = nullptr;

    NetworkRole role = NetworkRole::NONE;
    ConnectionState state = ConnectionState::DISCONNECTED;

    uint32_t localSequence = 0;
    uint32_t lastReceivedSequence = 0;

    PacketServerState latestServerState{};
    bool newServerStateAvailable = false;

    PacketClientInput latestClientInput{};
    bool newClientInputAvailable = false;

    void HandlePacket(const uint8_t* data, size_t size, uint8_t channelID);

public:
    ENetManager();
    ~ENetManager();

    bool StartHost(uint16_t port = DEFAULT_SERVER_PORT);
    bool StartClient(const std::string& hostIp, uint16_t port = DEFAULT_SERVER_PORT);
    void Update(float dt);

    void SendServerState(float bX, float bY, float bVx, float bVy, float p1Y, float p2Y, float p1Hp, float p2Hp);
    void SendClientInput(float paddleY, float paddleVelocityY);

    bool ConsumeServerState(PacketServerState& outState);
    bool ConsumeClientInput(PacketClientInput& outInput);
    void Disconnect();

    NetworkRole GetRole() const { return role; }
    ConnectionState GetState() const { return state; }
    bool IsConnected() const { return state == ConnectionState::CONNECTED; }
    int GetPingMs() const;
};

#endif // VSTRIKE_ENET_MANAGER_H
