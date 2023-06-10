#pragma once

#include <cstdarg>
#include <cstring>
#include <subhook.h>

#include "../Addresses.hpp"
#include "../api/Logging.hpp"

int PrintfHookFunc(const char *format, ...);

api::Logger *GetSRLogger();

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *printfHook;

int PrintfHookFunc(const char *format, ...)
{
    subhook::ScopedHookRemove scopedRemove(printfHook);

    if (std::strcmp(format, "connection successful\n") == 0)
    {
        GetSRLogger()->Log("<green>Connection successful<reset>.");
    }
    if (std::strcmp(format, "connection failed\n") == 0)
    {
        GetSRLogger()->Log("<red>Connection failed.");
    }
    else if (std::strcmp(format, "port:%d\n") == 0)
    {
        std::va_list va;
        va_start(va, format);
        GetSRLogger()->Log("Using port: {}.", va_arg(va, int));
        va_end(va);
    }
    else if (std::strcmp(format, "socketenabled:%d\n") == 0)
    {
        std::va_list va;
        va_start(va, format);
        GetSRLogger()->Log("Socket status: {}", va_arg(va, int) ? "<green>enabled<reset>." : "?");
        va_end(va);
    }

    return 0;
}

api::Logger *GetSRLogger()
{
    static api::Logger s = api::Logger("Sub Rosa", "<blue><b>");
    return &s;
}

#endif