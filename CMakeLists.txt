cmake_minimum_required(VERSION 3.21)

project("Suitium")

if (WIN32)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

add_subdirectory("Suitium")
if (WIN32)
	add_subdirectory("SuitiumLauncher")
endif ()