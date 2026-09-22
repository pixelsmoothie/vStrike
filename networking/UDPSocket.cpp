#include "UDPSocket.h"
#include <iostream>
#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define NATIVE_INVALID_SOCKET (INVALID_SOCKET)
    #define SOCKET_ERROR_CODE (WSAGetLastError())
#else
    #define PLATFORM_WINDOWS 0
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <cerrno>
    #define NATIVE_INVALID_SOCKET (-1)
    #define SOCKET_ERROR_CODE (errno)
#endif

bool UDPSocket::isWSAInitialized = false;

UDPSocket::UDPSocket() : sockHandle(static_cast<SocketHandle>(NATIVE_INVALID_SOCKET))
{
    GlobalInit();
}

UDPSocket::~UDPSocket()
{
    Close();
}

bool UDPSocket::GlobalInit()
{
#if PLATFORM_WINDOWS
    if (!isWSAInitialized)
    {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0)
        {
            std::cerr << "[Network] WSAStartup failed with error: " << result << std::endl;
            return false;
        }
        isWSAInitialized = true;
    }
#endif
    return true;
}

void UDPSocket::GlobalCleanup()
{
#if PLATFORM_WINDOWS
    if (isWSAInitialized)
    {
        WSACleanup();
        isWSAInitialized = false;
    }
#endif
}

bool UDPSocket::Open(uint16_t bindPort)
{
    Close();

    // 1. Create UDP socket
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET)
    {
        std::cerr << "[Network] Failed to create socket: " << SOCKET_ERROR_CODE << std::endl;
        sockHandle = static_cast<SocketHandle>(NATIVE_INVALID_SOCKET);
        return false;
    }
    sockHandle = static_cast<SocketHandle>(s);

    // 2. Bind to port if specified
    if (bindPort > 0)
    {
        sockaddr_in bindAddr{};
        bindAddr.sin_family = AF_INET;
        bindAddr.sin_addr.s_addr = INADDR_ANY;
        bindAddr.sin_port = htons(bindPort);

        if (bind(static_cast<SOCKET>(sockHandle), reinterpret_cast<sockaddr*>(&bindAddr), sizeof(bindAddr)) < 0)
        {
            std::cerr << "[Network] Failed to bind to port " << bindPort 
                      << ", error: " << SOCKET_ERROR_CODE << std::endl;
            Close();
            return false;
        }
        std::cout << "[Network] Socket successfully bound to port " << bindPort << std::endl;
    }

    SetNonBlocking(true);
    return true;
}

void UDPSocket::Close()
{
    if (sockHandle != static_cast<SocketHandle>(NATIVE_INVALID_SOCKET))
    {
#if PLATFORM_WINDOWS
        closesocket(static_cast<SOCKET>(sockHandle));
#else
        close(static_cast<int>(sockHandle));
#endif
        sockHandle = static_cast<SocketHandle>(NATIVE_INVALID_SOCKET);
    }
}

bool UDPSocket::IsOpen() const
{
    return sockHandle != static_cast<SocketHandle>(NATIVE_INVALID_SOCKET);
}

bool UDPSocket::SetNonBlocking(bool enable)
{
    if (sockHandle == static_cast<SocketHandle>(NATIVE_INVALID_SOCKET)) return false;

#if PLATFORM_WINDOWS
    u_long mode = enable ? 1 : 0;
    int result = ioctlsocket(static_cast<SOCKET>(sockHandle), FIONBIO, &mode);
    if (result != 0)
    {
        std::cerr << "[Network] ioctlsocket(FIONBIO) failed: " << WSAGetLastError() << std::endl;
        return false;
    }
#else
    int flags = fcntl(static_cast<int>(sockHandle), F_GETFL, 0);
    if (flags < 0) return false;
    flags = enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    if (fcntl(static_cast<int>(sockHandle), F_SETFL, flags) < 0) return false;
#endif

    isNonBlocking = enable;
    return true;
}

bool UDPSocket::SendTo(const std::string& destinationIp, uint16_t port, const void* data, size_t size)
{
    if (sockHandle == static_cast<SocketHandle>(NATIVE_INVALID_SOCKET)) return false;

    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);

#if PLATFORM_WINDOWS
    destAddr.sin_addr.s_addr = inet_addr(destinationIp.c_str());
#else
    inet_pton(AF_INET, destinationIp.c_str(), &destAddr.sin_addr);
#endif

    int bytesSent = sendto(
        static_cast<SOCKET>(sockHandle),
        reinterpret_cast<const char*>(data),
        static_cast<int>(size),
        0,
        reinterpret_cast<sockaddr*>(&destAddr),
        sizeof(destAddr)
    );

    return bytesSent == static_cast<int>(size);
}

int UDPSocket::RecvFrom(void* buffer, size_t maxLen, std::string& outSenderIp, uint16_t& outSenderPort)
{
    if (sockHandle == static_cast<SocketHandle>(NATIVE_INVALID_SOCKET)) return -1;

    sockaddr_in senderAddr{};
#if PLATFORM_WINDOWS
    int senderLen = sizeof(senderAddr);
#else
    socklen_t senderLen = sizeof(senderAddr);
#endif

    int bytesRead = recvfrom(
        static_cast<SOCKET>(sockHandle),
        reinterpret_cast<char*>(buffer),
        static_cast<int>(maxLen),
        0,
        reinterpret_cast<sockaddr*>(&senderAddr),
        &senderLen
    );

    if (bytesRead > 0)
    {
        outSenderIp = inet_ntoa(senderAddr.sin_addr);
        outSenderPort = ntohs(senderAddr.sin_port);
        return bytesRead;
    }

#if PLATFORM_WINDOWS
    int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK) return 0;
#else
    if (errno == EWOULDBLOCK || errno == EAGAIN) return 0;
#endif

    return -1;
}
