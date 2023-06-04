#include <bitset>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <type_traits>

// https://github.com/noche-x/client/blob/main/src/game.hpp
#if _WIN32
using DrawTextFunc = std::int64_t (*)(const char *, float, float, float, unsigned int, float, float, float, float, ...);
#elif __linux__
using DrawTextFunc = std::int64_t (*)(const char *, int, int, int, float, float, float, float, float, float, float, void *);
#endif

void CSFormat(const char *format, std::stringstream& newFormatStream, std::va_list vaList);
#if _WIN32
std::int64_t DrawTextHookFunc(const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...);
#elif __linux__
std::int64_t DrawTextHookFunc(const char *format, int, int, int, float, float, float, float, float, float, float, void *);
#endif

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *drawTextHook;

#if _WIN32
std::int64_t DrawTextHookFunc(const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);

    const AddressInterface::AddressMap addressTable = GetAddressInterface()->GetAddressMap();
    DrawTextFunc originalFunc = (DrawTextFunc)(GetBaseAddress() + addressTable.at(AddressType::DrawTextFunc));

    std::stringstream newFormatStream;
    if((flags & 0x40) == 0) // This flag makes the text unformatted
    {
        std::va_list vaList;
        va_start(vaList, alpha);
        CSFormat(format, newFormatStream, vaList);
        va_end(vaList);
    }
    else
        newFormatStream << format;

    unsigned int newFlags = flags;
    newFlags |= 0x40; // Add unformatted flag, since we already formatted it

    if (std::strcmp(format, "Sub Rosa") == 0)
    {
        // The main menu is being drawn!
        originalFunc("Suitium", x, y + 32, size * 0.85f, newFlags, 1.0f, 0.0f, 0.0f, 1.0f);
    }
    else if (std::strcmp(format, "W1nters") == 0)
    {
        // The last credits menu section is being drawn!
        originalFunc("Suitium is made by", x, y + 64, size, newFlags, 1.0f, 0.0f, 0.0f, 1.0f);
        originalFunc("JPXS", x + 120, y + 64, size, newFlags, 1.0f, 0.75f, 0.0f, 1.0f);
    }

    return originalFunc(newFormatStream.str().c_str(), x, y, size, newFlags, red, green, blue, alpha);
}
#elif __linux__
std::int64_t DrawTextHookFunc(const char *format, int, int, int, float, float, float, float, float, float, float, void *)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);
}
#endif

void CSFormat(const char *format, std::stringstream& newFormatStream, std::va_list vaList) 
{
    std::size_t formatSize = std::strlen(format);
    for (std::size_t formatCount = 0; formatCount < formatSize; formatCount++)
    {
        if (format[formatCount] != '/')
        {
            newFormatStream << format[formatCount];
            continue;
        }
        
        formatCount++;
        if (formatCount >= formatSize)
            break;
        
        // I know I could've used a switch here
        if (format[formatCount] == '/')
            newFormatStream << '/';
        else if (format[formatCount] == 'c')
            newFormatStream << va_arg(vaList, char);
        else if (format[formatCount] == 'i')
            newFormatStream << va_arg(vaList, int);
        else if (format[formatCount] == 'u')
            newFormatStream << va_arg(vaList, unsigned int);
        else if (format[formatCount] == 'x')
            newFormatStream << std::hex << va_arg(vaList, unsigned int) << std::dec;
        else if (format[formatCount] == 'f')
            newFormatStream << va_arg(vaList, double); // floats in varargs are always doubles!
        else if (format[formatCount] == 's')
            newFormatStream << va_arg(vaList, const char *);
        else if (format[formatCount] == 'l')
            newFormatStream << va_arg(vaList, std::int64_t);
        else if (format[formatCount] == 'L')
            newFormatStream << va_arg(vaList, std::uint64_t);
        else if (format[formatCount] == 'b')
        {
            // taken off stackoverflow, bitset to a stream prints binary representation
            unsigned int toFormat = va_arg(vaList, unsigned int);
            std::bitset<sizeof(int)> formatBitset(toFormat);
            newFormatStream << formatBitset;
        }
        else if (format[formatCount] == 'I') // this is number separated by commas, like 1,000,000
        {
            // implementation taken from game pseudocode
            int toFormat = va_arg(vaList, int);
            if (toFormat < 1000000)
            {
              if (toFormat < 1000)
              {
                newFormatStream << toFormat;
                continue;
              }
              else 
              {
                newFormatStream << toFormat / 1000 % 1000;
              }
            }
            else 
            {
                newFormatStream << toFormat / 1000000 << "," << toFormat / 1000 % 1000;
            }
            // Integer math, if toFormat / 1000 is a decimal it is floored at runtime
            newFormatStream << "," << std::setfill('0') << std::setw(3) << toFormat - (toFormat / 1000 * 1000);
        }
        else
        {
            newFormatStream = std::stringstream(); // Reset the stream, this is not supposed to happen
            newFormatStream << "Suitium Error: //" << format[formatCount] << " not implemented.";
            break;
        }
    }
}

#endif