cmake_minimum_required(VERSION 3.20)
project(dynamic_maze VERSION 0.1 LANGUAGES CXX)

# ─────────────────────────────────────────
# C++ Standard
# ─────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────
# Force Compiler (Only if NOT cross-compiling)
# ─────────────────────────────────────────
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_C_COMPILER clang)
endif()

# ─────────────────────────────────────────
# Build Type Toggling
# ─────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(BUILD_SHARED_LIBS OFF)

# ─────────────────────────────────────────
# Collect all source files
# ─────────────────────────────────────────
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(dynamic_maze ${SOURCES})

target_include_directories(dynamic_maze PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/include
)

# ─────────────────────────────────────────
# PLATFORM-SPECIFIC LINKING
# ─────────────────────────────────────────
if(WIN32)
    # Windows (MinGW) Linking: Point directly to the custom compiled .a file
    target_link_libraries(dynamic_maze PRIVATE 
        ${CMAKE_SOURCE_DIR}/src/lib/win/libraylib.a 
        winmm gdi32 opengl32
    )
else()
    # Linux Native Linking: Use the system package manager
    find_package(raylib REQUIRED)
    
    if(TARGET raylib::raylib)
        target_link_libraries(dynamic_maze PRIVATE raylib::raylib)
    else()
        target_link_libraries(dynamic_maze PRIVATE raylib)
    endif()
    
    target_link_libraries(dynamic_maze PRIVATE GL m pthread dl rt X11)
endif()