# init
cmake_minimum_required(VERSION 3.5)
set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})
include(msvc)
include(gcc)

# Generates compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++17 for optional
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(Typemap LANGUAGES CXX)

# Fix msvc
msvc_remove_warning_level()
msvc_set_flags_if("/std:c++17  /utf-8")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)

msvc_set_flags_if("/W4")
gcc_set_flags_if("-Wall -Wextra")

add_subdirectory("lib/")

option(UNIT_TEST "Build unit tests" OFF)
if (UNIT_TEST)
    enable_testing()
    add_subdirectory("tests/")
endif()
