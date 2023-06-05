#pragma once

#include <cstdint>
#include <type_traits>

template<class T>
struct DataAddress
{
    T *ptr = nullptr;

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
    auto operator()(Args... args)
    {
        return this->ptr(std::forward<Args>(args)...);
    }

    void Register(std::uintptr_t iptr)
    {
        this->ptr = (F *)iptr;
    }
};