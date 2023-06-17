#pragma once

#include <cstdint>
#include <subhook.h>

extern subhook::Hook *drawTextHook;

// https://github.com/noche-x/client/blob/main/src/game.hpp
#if _WIN32
std::int64_t CSDrawTextHookFunc(const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...);
#elif __linux__
std::int64_t CSDrawTextHookFunc(const char *format, int, int, int, float, float, float, float, float, float, float, void *);
#endif

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

#include <bitset>
#include <cstdarg>
#include <fmt/format.h>
#include <iomanip>
#include <sstream>

#include "../Addresses.hpp"
#include "../Version.hpp"

subhook::Hook *drawTextHook;

#if _WIN32
static void csFormat(const char *format, std::stringstream& newFormatStream, std::va_list vaList) 
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

std::int64_t CSDrawTextHookFunc(const char *format, float x, float y, float size, unsigned int flags, float red, float green, float blue, float alpha, ...)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);

    std::stringstream newFormatStream;
    if((flags & 0x40) == 0) // This flag makes the text unformatted
    {
        std::va_list vaList;
        va_start(vaList, alpha);
        csFormat(format, newFormatStream, vaList);
        va_end(vaList);
    }
    else
        newFormatStream << format;

    unsigned int newFlags = flags;
    newFlags |= 0x40; // Add unformatted flag, since we already formatted it

    std::uintptr_t returnAddress = (std::uintptr_t)_ReturnAddress() - (std::uintptr_t)addresses::Base.ptr;
    if (returnAddress == 0x986D8) // 0x986D8 is the instruction after the game renders the "Sub Rosa" main menu text
    {
        return 0; // Remove "Sub Rosa" text
    }
    else if (returnAddress == 0x9873D) // 0x9873D is the instruction after the game renders the version main menu text
    {
        return 0; // Remove version text
    }
    else if (returnAddress == 0xFCFD2) // 0xFCFD2 is the instruction after the game renders the last credits section
    {
        // The last credits menu section is being drawn!
        addresses::CSDrawTextFunc("Suitium is made by", x, y + 64, size, newFlags, 1.0f, 0.0f, 0.0f, 1.0f);
        addresses::CSDrawTextFunc("JPXS", x + 120, y + 64, size, newFlags, 1.0f, 0.75f, 0.0f, 1.0f);
    }
    else if (returnAddress == 0x9658E) // 0x9658E is the instruction after the game renders the "generating" text
    {
        return addresses::CSDrawTextFunc("Generating...", x, y, size, newFlags, red, green, blue, alpha);
    }
    else if (returnAddress == 0x966B4) // 0x966B4 is the instruction after the game renders the "connecting" text
    {
        return addresses::CSDrawTextFunc("Connecting...", x, y, size, newFlags, red, green, blue, alpha);
    }

    return addresses::CSDrawTextFunc(newFormatStream.str().c_str(), x, y, size, newFlags, red, green, blue, alpha);
}
#elif __linux__
std::int64_t CSDrawTextHookFunc(const char *format, int params, int a, int b, float x, float y, float scale, float red, float green, float blue, float alpha, void * c)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);

    return addresses::CSDrawTextFunc(format, params, a, b, x, y, scale, red, green, blue, alpha, c);
}
#endif

#endif