#include "MasterServer.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <httplib.h>
#include <sstream>

#include "Addresses.hpp"

#if _WIN32
#include <WinSock2.h>
#elif __linux__
#include <unistd.h>
#endif

MasterServer::MasterServer()
{
#if _WIN32
    this->_socket = INVALID_SOCKET;
#elif __linux__
    this->_socket = -1;
#endif

    this->_validClient = false;
}
MasterServer::~MasterServer()
{
}

MasterServer *MasterServer::GetSingleton()
{
    static MasterServer s;
    return &s;
}

bool MasterServer::IsConnected() const
{
#if _WIN32
    return this->_socket != INVALID_SOCKET;
#elif __linux__
    return this->_socket != -1;
#endif
}
bool MasterServer::Connect(std::string providerAddress)
{
    httplib::Client providerClient(providerAddress);

    httplib::Result result = providerClient.Get("/anewzero/serverinfo.php");
    if (result->status != 200) // 200 OK
        return false;

    int gameVersion; // not used
    std::string serverAddress;
    unsigned short serverPort;

    std::stringstream infoStream;
    int infoCount = 0;

    std::string body = result->body;
    while (body.front() == '\t') // remove all beginning tabs
        body.erase(body.begin());

    for (auto bodyIt = body.begin(); bodyIt != body.end(); ++bodyIt)
    {
        if (*bodyIt == '\t' || bodyIt == body.end() - 1)
        {
            if (bodyIt == body.end() - 1)
                infoStream << *bodyIt;

            if (infoCount == 0)
                gameVersion = std::stoi(infoStream.str());
            else if (infoCount == 1)
                serverAddress = infoStream.str();
            else if (infoCount == 2)
                serverPort = std::stoi(infoStream.str()) + 2; // + 0 is <=24 servers, 1 is unknown, 2 is >=35

            infoStream = std::stringstream();
            infoCount++;

            continue;
        }

        infoStream << *bodyIt;
    }

#if _WIN32
    this->_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->_socket == INVALID_SOCKET)
        return false;

    this->_socketAddress = {};
    this->_socketAddress.sin_family = AF_INET;
    this->_socketAddress.sin_addr.S_un.S_addr = inet_addr(serverAddress.c_str());
    this->_socketAddress.sin_port = htons(serverPort);
#elif __linux__
    // TODO:
#endif

    this->_validClient = false;

    return true;
}
void MasterServer::Disconnect()
{
    if (!this->IsConnected())
        return;

#if _WIN32
    closesocket(this->_socket);
    this->_socket = INVALID_SOCKET;
#elif __linux__
    close(this->_socket);
    this->_socket = -1;
#endif
}

bool MasterServer::IsClientValid() const
{
    return this->IsConnected() && this->_validClient;
}
std::string MasterServer::GetClientName() const
{
    if (!this->IsClientValid())
       return "?";
    return addresses::AuthName.ptr;
}
std::int32_t MasterServer::GetClientPurchaseID() const
{
    if (!this->IsClientValid())
       return -1;
    return this->_clientPurchaseID;
}
std::uint32_t MasterServer::GetClientPhoneNumber() const
{
    if (!this->IsClientValid())
       return 0;
    return this->_clientPhoneNumber;
}

void MasterServer::RequestClientInfo()
{
    if (!this->IsConnected())
        return;
    if (*addresses::IsDedicated.ptr)
        return;

    // Send
    {
        std::string authName = addresses::AuthName.ptr;
        int ticketLength = *addresses::SteamTicketLength.ptr;
        char *ticketBuffer = addresses::SteamTicketBuffer.ptr;

        // 5 = magic
        // 32 = name
        // 4 = ticketlength
        std::size_t packetSize = 5 + 32 + 4 + ticketLength;
        char *packetBuffer = (char *)std::malloc(packetSize);
        std::memset(packetBuffer, 0, packetSize);
        std::memcpy(&packetBuffer[0], "7DFPH", 5);
        std::memcpy(&packetBuffer[5], authName.c_str(), authName.size() /* <32*/);
        std::memcpy(&packetBuffer[37], &ticketLength, 4);
        std::memcpy(&packetBuffer[41], ticketBuffer, (std::size_t)ticketLength);

        sendto(this->_socket, packetBuffer, packetSize, 0, (const sockaddr *)&this->_socketAddress, sizeof(sockaddr_in));

        std::free(packetBuffer);
    }

    // Receive
    {
        // 5 = magic
        // 4 = game ID
        // 4 = master auth
        // 4 = phone number
        std::size_t packetSize = 5 + 4 + 4 + 4;
        char *packetBuffer = (char *)std::malloc(packetSize);

        sockaddr_in fromAddress = {};
        int fromAddressLen = sizeof(sockaddr_in);
        if (recvfrom(this->_socket, packetBuffer, packetSize, 0, (sockaddr *)&fromAddress, &fromAddressLen) != packetSize) // TODO: timeout
            goto label_bad;
        if (memcmp(&fromAddress, &this->_socketAddress, sizeof(sockaddr_in)) != 0)
            goto label_bad; // Packet is not from master server
        if (memcmp(&packetBuffer[0], "7DFPH", 5) != 0)
            goto label_bad; // Invalid magic
        
        this->_clientPurchaseID = *(std::int32_t *)(&packetBuffer[5]);
        this->_clientPhoneNumber = *(std::uint32_t *)(&packetBuffer[13]);
        this->_validClient = true;

        if (this->_clientPhoneNumber == 0) // looks like the ticket wasn't valid
            this->_validClient = false;

label_bad: {}
        std::free(packetBuffer);
    }
}