#include "AddressInterface.hpp"

#include "AddressInterfaceWin32Client.hpp"

static std::intptr_t baseAddress;

AddressInterface::AddressInterface()
{
}
AddressInterface::~AddressInterface()
{
}

AddressInterface *GetAddressInterface()
{
    // TODO: dedicated interfaces

#if _WIN32
    static AddressInterfaceWin32Client s;
    return &s;
#endif
}

void SetBaseAddress(std::intptr_t address)
{
    baseAddress = address;
}
std::intptr_t GetBaseAddress()
{
    return baseAddress;
}