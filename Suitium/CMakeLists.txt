cmake_minimum_required(VERSION 3.21)

add_library(
    Suitium SHARED

    # Main source files
    Win32.cpp
    Linux.cpp

    # API source files
    api/Logging.cpp

    # Other source files
    Addon.cpp
    Address.cpp
    Addresses.cpp
    Hooks.cpp
    Hotfixes.cpp
    Version.cpp

    # Libraries
    ${CMAKE_SOURCE_DIR}/fmt/src/format.cc
    ${CMAKE_SOURCE_DIR}/subhook/subhook.c
        Linux.cpp)

set_property (TARGET Suitium PROPERTY CXX_STANDARD 17)

if (WIN32)
    target_compile_definitions(Suitium PRIVATE SUBHOOK_STATIC) # Windows will give "dllimport" errors without this define.
    target_link_libraries(Suitium "Ws2_32")
    target_link_libraries(Suitium "${CMAKE_SOURCE_DIR}/Output/moonjit/src/lua51.lib" "${CMAKE_SOURCE_DIR}/Output/moonjit/src/luajit.lib")
endif ()

target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/fmt/include")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/glm")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/json/include")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/moonjit")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/subhook")