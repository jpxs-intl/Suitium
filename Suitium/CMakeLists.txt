cmake_minimum_required(VERSION 3.21)

add_library(
    Suitium SHARED
    Win32.cpp
)

set_property (TARGET Suitium PROPERTY CXX_STANDARD 17)

