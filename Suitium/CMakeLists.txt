cmake_minimum_required(VERSION 3.21)

add_library(
    Suitium SHARED

    # Main source files
    Win32.cpp

    # Other source files
    Address.cpp
    Addresses.cpp
    Hooks.cpp
    Hotfixes.cpp
    Version.cpp

    # Libraries
    ${CMAKE_SOURCE_DIR}/subhook/subhook.c
    ${CMAKE_SOURCE_DIR}/fmt/src/format.cc
)

set_property (TARGET Suitium PROPERTY CXX_STANDARD 17)

if (WIN32)
    target_compile_definitions(Suitium PRIVATE SUBHOOK_STATIC) # Windows will give "dllimport" errors without this define.
    target_link_libraries(Suitium Ws2_32)
endif ()

target_include_directories(Suitium PRIVATE ${CMAKE_SOURCE_DIR}/subhook)
target_include_directories(Suitium PRIVATE ${CMAKE_SOURCE_DIR}/fmt/include)