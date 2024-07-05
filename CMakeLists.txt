cmake_minimum_required(VERSION 3.15)
project(maxc-utils)

# C++ standard version
set(CMAKE_CXX_STANDARD 20 CACHE STRING "" FORCE)

# Executable target
add_executable(main)
set_target_properties(main PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# Add compiler definitions
target_compile_definitions(main PRIVATE $<$<CONFIG:Debug>:DEBUG=1>)
target_compile_definitions(main PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

# Includes & pre-compiled header
target_include_directories(main PRIVATE "./src/")
target_precompile_headers(main PRIVATE "./src/pch.h")

# Glob all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
target_sources(main PRIVATE ${SOURCES})
