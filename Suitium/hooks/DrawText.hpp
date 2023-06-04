#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <type_traits>
#include <iomanip>
#include <bitset>

// https://github.com/noche-x/client/blob/main/src/game.hpp
#if _WIN32
using DrawTextFunc = std::int64_t (*)(const char *, float, float, float, int, float, float, float, float, ...);
#elif __linux__
using DrawTextFunc = std::int64_t (*)(const char *, int, int, int, float, float, float, float, float, float, float, void *);
#endif

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

void gameFormatText(const char *format, std::stringstream& newFormatStream, va_list vaList) 
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

        if (format[formatCount] == '/')
            newFormatStream << "//";
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
              else {
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

#if IMPLEMENT_HOOKS

static subhook::Hook *drawTextHook;

#if _WIN32
std::int64_t DrawTextHookFunc(const char *format, float arg2, float arg3, float arg4, int flags, float arg6, float arg7, float arg8, float arg9, ...)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);

    const AddressInterface::AddressTable addressTable = GetAddressInterface()->GetAddressTable();
    DrawTextFunc originalFunc = (DrawTextFunc)(GetBaseAddress() + addressTable.at(AddressType::DrawTextFunc));

    std::stringstream newFormatStream;
    
    if((flags & 0x40) == 0)
    {
        va_list vaList;
        va_start(vaList, arg9);

        gameFormatText(format, newFormatStream, vaList);

        va_end(vaList);
    }
    else
        newFormatStream << format;

    return originalFunc(newFormatStream.str().c_str(), arg2, arg3, arg4, flags, arg6, arg7, arg8, arg9);
}
#elif __linux__
std::int64_t DrawTextHookFunc(const char *, int, int, int, float, float, float, float, float, float, float, void *)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);
}
#endif

#endif