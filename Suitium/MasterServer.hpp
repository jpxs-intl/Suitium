#pragma once

#include <cstdint>
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
    std::int32_t _clientPurchaseID; // this is a sequential ID, generated after the player launches the game for the first time
    std::uint32_t _clientPhoneNumber;
public:
    bool IsClientValid() const;
    std::string GetClientName() const;
    std::int32_t GetClientPurchaseID() const;
    std::uint32_t GetClientPhoneNumber() const;

    void RequestClientInfo();
};