#include <cstdint>
#include <type_traits>

// https://github.com/noche-x/client/blob/main/src/game.hpp
#if _WIN32
using DrawTextFunc = std::int64_t (*)(char *, float, float, float, int, float, float, float, float, ...);
#elif __linux__
using DrawTextFunc = std::int64_t (*)(char *, int, int, int, float, float, float, float, float, float, float, void *);
#endif

// I need stuff!!!!
#if _VSCODE
#define IMPLEMENT_HOOKS 1
#endif

#if IMPLEMENT_HOOKS

static subhook::Hook *drawTextHook;

#if _WIN32
template<typename... Args>
std::int64_t DrawTextHookFunc(char *arg1, float arg2, float arg3, float arg4, int arg5, float arg6, float arg7, float arg8, float arg9, Args&&... args)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);

    const AddressInterface::AddressTable addressTable = GetAddressInterface()->GetAddressTable();
    DrawTextFunc originalFunc = (DrawTextFunc)(GetBaseAddress() + addressTable.at(AddressType::DrawTextFunc));
    originalFunc(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, std::forward<Args>(args)...);

    return 0;
}
#elif __linux__
std::int64_t DrawTextHookFunc(char *, int, int, int, float, float, float, float, float, float, float, void *)
{
    subhook::ScopedHookRemove scopedRemove(drawTextHook);
}
#endif

#endif