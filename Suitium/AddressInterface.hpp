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

    using AddressMap = std::unordered_map<AddressType, std::intptr_t>;

    virtual bool CheckVersion() const = 0;
    virtual const AddressMap &GetAddressMap() const = 0;
};

AddressInterface *GetAddressInterface();

void RegisterBaseAddress(std::intptr_t address);
std::intptr_t GetBaseAddress(void);