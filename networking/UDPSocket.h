#ifndef VSTRIKE_UDP_SOCKET_H
#define VSTRIKE_UDP_SOCKET_H

#include <string>
#include <cstdint>
#include <cstddef>

// Opaque socket handle to completely decouple OS headers from game engine headers
typedef uintptr_t SocketHandle;

// ============================================================================
// UDPSocket: Cross-platform, Non-blocking UDP Socket Wrapper
// ============================================================================
class UDPSocket
{
private:
    SocketHandle sockHandle;
    bool isNonBlocking = false;
    static bool isWSAInitialized;

public:
    UDPSocket();
    ~UDPSocket();

    // OS Socket Subsystem Lifecycle (WSAStartup / WSACleanup)
    static bool GlobalInit();
    static void GlobalCleanup();

    // Socket Operations
    bool Open(uint16_t bindPort = 0);
    void Close();
    bool IsOpen() const;

    // Non-blocking mode configuration (FIONBIO / O_NONBLOCK)
    bool SetNonBlocking(bool enable = true);

    // Send binary packet to destination IP and Port
    bool SendTo(const std::string& destinationIp, uint16_t port, const void* data, size_t size);

    // Receive binary packet (returns bytes received, 0 if no packet waiting, -1 on fatal error)
    int RecvFrom(void* buffer, size_t maxLen, std::string& outSenderIp, uint16_t& outSenderPort);
};

#endif // VSTRIKE_UDP_SOCKET_H
