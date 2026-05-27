cmake_minimum_required(VERSION 3.20)
project(dynamic_maze VERSION 0.1 LANGUAGES CXX)

# ─────────────────────────────────────────
# C++ Standard
# ─────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────
# Compiler
# ─────────────────────────────────────────
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

# ─────────────────────────────────────────
# Build Type
# ─────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    
endif()

# ─────────────────────────────────────────
# Compiler Flags
# ─────────────────────────────────────────
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
# CMakeLists.txt — add debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address -fno-omit-frame-pointer")

# CMakeLists.txt
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# ─────────────────────────────────────────
# Find Raylib (system-wide)
# ─────────────────────────────────────────
find_package(raylib REQUIRED)

# ─────────────────────────────────────────
# Collect all source files
# ─────────────────────────────────────────
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# ─────────────────────────────────────────
# Executable
# ─────────────────────────────────────────
# make sure ALL .cpp files are listed
add_executable(dynamic_maze ${SOURCES})

# ─────────────────────────────────────────
# Include Directories
# ─────────────────────────────────────────
target_include_directories(dynamic_maze PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# ─────────────────────────────────────────
# Link Raylib
# ─────────────────────────────────────────
target_link_libraries(dynamic_maze PRIVATE raylib)

# ─────────────────────────────────────────
# Copy assets to build directory
# ─────────────────────────────────────────
add_custom_command(TARGET dynamic_maze POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:dynamic_maze>/assets
)
