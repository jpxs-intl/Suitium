#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#if _WIN32
#define DYNADDR(windows, linux) windows
#elif __linux__
#define DYNADDR(windows, linux) linux
#endif

template<class T>
struct DataAddress
{
    T *ptr = nullptr;

    auto &operator *() const
    {
        return *ptr;
    }
    auto &operator [](std::size_t n) const
    {
        return ptr[n];
    }

    void Register(std::uintptr_t iptr)
    {
        this->ptr = (T *)iptr;
    }
};

template<typename F>
struct FuncAddress
{
    F *ptr = nullptr;

    template<typename... Args>
    auto operator()(Args&&... args) const
    {
        return this->ptr(std::forward<Args>(args)...);
    }

    void Register(std::uintptr_t iptr)
    {
        this->ptr = (F *)iptr;
    }
};