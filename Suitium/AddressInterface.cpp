#include "AddressInterface.hpp"

#include "AddressInterfaceWin32Client.hpp"

static std::intptr_t baseAddress = 0;

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

void RegisterBaseAddress(std::intptr_t address)
{
    if (baseAddress == 0) // why not?
        baseAddress = address;
}
std::intptr_t GetBaseAddress()
{
    return baseAddress;
}