cmake_minimum_required(VERSION 3.21)

add_library(
    Suitium SHARED

    # Main source files
    Win32.cpp

    # Other source files
    Hooks.cpp
    AddressInterface.cpp
    AddressInterfaceWin32Client.cpp

    # Libraries
    ../subhook/subhook.c
)

set_property (TARGET Suitium PROPERTY CXX_STANDARD 20)

if (WIN32)
    target_compile_definitions(Suitium PRIVATE SUBHOOK_STATIC) # Windows will give "dllimport" errors without this define.
endif ()

target_include_directories(Suitium PRIVATE ../subhook)