#pragma once

#include <cstdint>
#include <unordered_map>

#include "AddressInterface.hpp"

class AddressInterfaceWin32Client : public AddressInterface
{
public:
    AddressInterfaceWin32Client();
    virtual ~AddressInterfaceWin32Client() noexcept;

    AddressInterfaceWin32Client(const AddressInterfaceWin32Client &) = delete;

    virtual bool CheckVersion() const override;
    virtual const std::unordered_map<AddressType, intptr_t> &GetAddressTable() const override;
};