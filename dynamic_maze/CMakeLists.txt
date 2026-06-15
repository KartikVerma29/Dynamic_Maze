cmake_minimum_required(VERSION 3.20)
project(dynamic_maze VERSION 0.1 LANGUAGES CXX)

# ─────────────────────────────────────────
# C++ Standard
# ─────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────
# Force Compiler (Enforce Clang)
# ─────────────────────────────────────────
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

# ─────────────────────────────────────────
# Build Type Toggling
# ─────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# ─────────────────────────────────────────
# FORCE STATIC LINKING PROPERTIES
# ─────────────────────────────────────────
# Tells CMake to favor static libraries (.lib/.a) globally
set(BUILD_SHARED_LIBS OFF)

# Find Raylib (system-wide configuration)
find_package(raylib REQUIRED)

# ─────────────────────────────────────────
# Collect all source files
# ─────────────────────────────────────────
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# ─────────────────────────────────────────
# Executable Creation
# ─────────────────────────────────────────
add_executable(dynamic_maze ${SOURCES})

target_include_directories(dynamic_maze PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# ─────────────────────────────────────────
# Link Raylib Statically
# ─────────────────────────────────────────
# Force linking to the explicit static target if exposed by your package manager
if(TARGET raylib::raylib)
    target_link_libraries(dynamic_maze PRIVATE raylib::raylib)
else()
    target_link_libraries(dynamic_maze PRIVATE raylib)
endif()

# On Windows/Linux, static linking requires standard system dependencies 
# to be attached explicitly alongside Raylib
if(WIN32)
    target_link_libraries(dynamic_maze PRIVATE winmm gdi32 opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(dynamic_maze PRIVATE GL m pthread dl rt X11)
endif()