#ifndef VSTRIKE_NETWORK_PROTOCOL_H
#define VSTRIKE_NETWORK_PROTOCOL_H

#include <cstdint>

constexpr uint32_t VSTRIKE_NET_MAGIC = 0x56535452; // "VSTR"
constexpr uint16_t DEFAULT_SERVER_PORT = 7777;

enum class PacketType : uint8_t
{
    HANDSHAKE_REQUEST = 1,
    HANDSHAKE_ACCEPT  = 2,
    CLIENT_INPUT      = 3,
    SERVER_STATE      = 4,
    PING              = 5,
    PONG              = 6,
    DISCONNECT        = 7
};

#pragma pack(push, 1)

struct PacketHeader
{
    uint32_t magic = VSTRIKE_NET_MAGIC;
    PacketType type;
    uint32_t sequenceNumber;
};

struct PacketHandshake
{
    PacketHeader header;
    uint8_t assignedPlayerId;
};

struct PacketClientInput
{
    PacketHeader header;
    float paddleY;
    float paddleVelocityY;
    uint32_t clientTimestamp;
};

struct PacketServerState
{
    PacketHeader header;
    float ballX;
    float ballY;
    float ballVx;
    float ballVy;
    float paddle1Y;
    float paddle2Y;
    float paddle1Hp;
    float paddle2Hp;
    uint32_t serverTick;
};

struct PacketPingPong
{
    PacketHeader header;
    uint32_t clientTimestampMs;
};

#pragma pack(pop)

#endif // VSTRIKE_NETWORK_PROTOCOL_H
