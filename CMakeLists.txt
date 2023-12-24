## Root CMake file
###############################################################################

cmake_minimum_required(VERSION 3.12)

project(
	xendit-c
	VERSION 1.0.0
	DESCRIPTION "Unofficial Xendit REST API Client for C and C++"
)

## Global includes
include(CTest)
enable_testing()

## Dependencies
find_package(CURL REQUIRED)
find_package(json-c REQUIRED)

## Flags
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -O3")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -DDEBUG")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

## Build variables
set(XND_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set(XND_SRC_DIRECTORY     ${PROJECT_SOURCE_DIR}/src)
set(XND_TESTS_DIRECTORY   ${PROJECT_SOURCE_DIR}/tests)
set(XND_STATIC_LIBRARY    ${PROJECT_NAME}-static)

## Traverse subdirectories
add_subdirectory(${XND_INCLUDE_DIRECTORY})
add_subdirectory(${XND_SRC_DIRECTORY})
add_subdirectory(${XND_TESTS_DIRECTORY})
