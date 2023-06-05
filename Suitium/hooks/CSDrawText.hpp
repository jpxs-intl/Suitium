#include <bitset>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <format>
#include <iomanip>
#include <sstream>
#include <subhook.h>
#include <type_traits>

#include "../Addresses.hpp"
#include "../MasterServer.hpp"
#include "../Version.hpp"

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

static subhook::Hook *drawTextHook;

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

#if _WIN32
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

    if (flags != newFlags) // This is to make sure the text is "constant"
    {
        if (std::strcmp(format, "Sub Rosa") == 0)
        {
            // The main menu is being drawn!
  
            // TODO: This probably shouldnt be handled here...
            int *authStatus = addresses::AuthStatus.ptr;
            int *ticketRetrieved = addresses::SteamTicketRetrieved.ptr;
            if (*addresses::AuthStatus.ptr == 0 && *addresses::SteamTicketRetrieved.ptr)
            {
                *addresses::AuthStatus.ptr = 3; // We don't need the vanilla game anymore
                MasterServer::GetSingleton()->RequestClientInfo();

                *addresses::SteamEnabled.ptr = 0; // I'm not sure if this affects anything, but it's here to hide the "Steam" text
            }

            int *steamEnabled = addresses::SteamEnabled.ptr;
            if (MasterServer::GetSingleton()->IsConnected())
            {
                // TODO: these colors might be not that good
                if (MasterServer::GetSingleton()->IsClientValid())
                    addresses::CSDrawTextFunc(std::format("Welcome, {}!", MasterServer::GetSingleton()->GetClientName()).c_str(), x, y + 95, size * 1.5f, newFlags, 0.0f, 1.0f, 0.0f, 1.0f);
                else if (!*addresses::SteamTicketRetrieved.ptr && *addresses::SteamEnabled.ptr)
                    addresses::CSDrawTextFunc("Hold up...", x, y + 95, size * 1.5f, newFlags, 1.0f, 1.0f, 0.0f, 1.0f);
                else if (!*addresses::SteamTicketRetrieved.ptr && !*addresses::SteamEnabled.ptr)
                    addresses::CSDrawTextFunc("Uh-oh! Looks like Steam is not open.", x, y + 95, size * 1.5f, newFlags, 1.0f, 0.0f, 0.0f, 1.0f);
                else
                {
                    addresses::CSDrawTextFunc("Uh-oh! Looks like you don't own the game.", x, y + 95, size * 1.5f, newFlags, 1.0f, 0.0f, 0.0f, 1.0f);
                    addresses::CSDrawTextFunc("Piracy is no party!", x, y + 120, size * 0.9f, newFlags, 1.0f, 0.35f, 0.0f, 1.0f);
                }
            }
            else
                addresses::CSDrawTextFunc("Uh-oh! Suitium could not connect to the master server.", x, y + 95, size * 1.5f, newFlags, 1.0f, 0.0f, 0.0f, 1.0f);

            addresses::CSDrawTextFunc(std::format("Suitium {}", SUITIUM_VERSION).c_str(), x, y + 14.5f, size * 0.85f, newFlags, 1.0f, 0.0f, 0.0f, 1.0f);
            return addresses::CSDrawTextFunc(std::format("Sub Rosa 0.{}{}", *addresses::GameVersionNumber.ptr, (char)(*addresses::GameVersionPatchNumber.ptr + 97)).c_str(), x, y, size * 1.25f, newFlags, red, green, blue, alpha);
        }
        else if (std::strcmp(newFormatStream.str().c_str(), std::format("ALPHA {}{}", *addresses::GameVersionNumber.ptr, (char)(*addresses::GameVersionPatchNumber.ptr + 97)).c_str()) == 0)
        {
            return 0; // Remove version text
        }
        else if (std::strcmp(format, "W1nters") == 0)
        {
            // The last credits menu section is being drawn!
            addresses::CSDrawTextFunc("Suitium is made by", x, y + 64, size, newFlags, 1.0f, 0.0f, 0.0f, 1.0f);
            addresses::CSDrawTextFunc("JPXS", x + 120, y + 64, size, newFlags, 1.0f, 0.75f, 0.0f, 1.0f);
        }
        else if (std::strcmp(format, "generating") == 0)
        {
            return addresses::CSDrawTextFunc("Generating...", x, y, size, newFlags, red, green, blue, alpha);
        }
        else if (std::strcmp(format, "connecting") == 0)
        {
            return addresses::CSDrawTextFunc("Connecting...", x, y, size, newFlags, red, green, blue, alpha);
        }
    }

    return addresses::CSDrawTextFunc(newFormatStream.str().c_str(), x, y, size, newFlags, red, green, blue, alpha);
}
#elif __linux__
std::int64_t CSDrawTextHookFunc(const char *format, int, int, int, float, float, float, float, float, float, float, void *)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);
}
#endif

#endif