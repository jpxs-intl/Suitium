#pragma once

#include <cstdint>
#include <unordered_map>

enum class AddressType
{
    // Variables

    GameVersionNum,
    GameVersionChar,

    // Functions

    DrawTextFunc,

    _Count
};

class AddressInterface
{
public:
    AddressInterface();
    virtual ~AddressInterface() noexcept = 0;

    AddressInterface(const AddressInterface &) = delete;

    using AddressTable = std::unordered_map<AddressType, std::intptr_t>;

    virtual bool CheckVersion() const = 0;
    virtual const AddressTable &GetAddressTable() const = 0;
};

AddressInterface *GetAddressInterface();

void SetBaseAddress(std::intptr_t address);
std::intptr_t GetBaseAddress(void);