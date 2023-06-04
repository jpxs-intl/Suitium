#pragma once

#include <string>

#if _WIN32
#include <WinSock2.h>
#elif __linux__
#include <unistd.h>
#endif

class MasterServer
{
private:
    MasterServer();
    ~MasterServer() noexcept;

public:
    MasterServer(const MasterServer &) = delete;

    static MasterServer *GetSingleton();

    bool IsConnected() const;
    bool Connect(std::string providerAddress);
    void Disconnect();
private:
    int _socket;
    sockaddr_in _socketAddress;

    bool _validClient;
public:
    bool IsClientValid() const;
    std::string GetClientName() const;

    void RequestClientInfo();
};