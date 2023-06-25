cmake_minimum_required(VERSION 3.21)

add_library(
    Suitium SHARED

    # Main source files
    Win32.cpp
    Linux.cpp

    # API source files
    api/Logging.cpp
    api/Text.cpp

    # Struct source files
    structs/Human.cpp
    structs/Item.cpp
    structs/ItemType.cpp
    structs/Vehicle.cpp
    structs/VehicleType.cpp

    # Other source files
    Addon.cpp
    Address.cpp
    Addresses.cpp
    Hooks.cpp
    Hotfixes.cpp
    LuaManager.cpp
    TypeManager.cpp
    Version.cpp

    # Libraries
    ${CMAKE_SOURCE_DIR}/Modules/fmt/src/format.cc
    ${CMAKE_SOURCE_DIR}/Modules/glad/glad.c
    ${CMAKE_SOURCE_DIR}/Modules/subhook/subhook.c
)

set_property (TARGET Suitium PROPERTY CXX_STANDARD 17)

if (WIN32)
    target_compile_definitions(Suitium PRIVATE SUBHOOK_STATIC) # Windows will give "dllimport" errors without this define.
    target_link_libraries(Suitium "Ws2_32")
    target_link_libraries(Suitium "${CMAKE_SOURCE_DIR}/Output/moonjit/src/lua51.lib" "${CMAKE_SOURCE_DIR}/Output/moonjit/src/luajit.lib")
endif ()

target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/Shared")

target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/Modules/fmt/include")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/Modules/glad/include")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/Modules/glm")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/Modules/json/include")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/Modules/moonjit/src")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/Modules/sol2/include")
target_include_directories(Suitium PRIVATE "${CMAKE_SOURCE_DIR}/Modules/subhook")