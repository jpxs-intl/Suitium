cmake_minimum_required(VERSION 3.21)

project("Suitium")

add_subdirectory("Suitium")
if (WIN32)
	add_subdirectory("SuitiumLauncher")
endif ()