cmake_minimum_required(VERSION 3.20)

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

project(dynamic_maze VERSION 0.1 LANGUAGES CXX)
# ─────────────────────────────────────────
# Build Type Toggling
# ─────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fno-omit-frame-pointer")
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
)

target_include_directories(dynamic_maze SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
)

# ─────────────────────────────────────────
# Profiling (Tracy), native builds only
# ─────────────────────────────────────────
option(ENABLE_PROFILING "Enable Tracy profiler" OFF)

if(ENABLE_PROFILING AND NOT EMSCRIPTEN AND NOT WIN32)
    set(TRACY_ENABLE ON CACHE BOOL "" FORCE)
    set(TRACY_ON_DEMAND ON CACHE BOOL "" FORCE)   # no buffering until the GUI connects
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/tracy)
    target_link_libraries(dynamic_maze PRIVATE Tracy::TracyClient)
    target_compile_definitions(dynamic_maze PRIVATE PROFILING_ENABLED)
    target_compile_options(dynamic_maze PRIVATE -fno-omit-frame-pointer)
endif()

# ─────────────────────────────────────────
# PLATFORM-SPECIFIC LINKING
# ─────────────────────────────────────────
if(WIN32)
    # Windows (MinGW) Linking
    target_link_libraries(dynamic_maze PRIVATE 
        ${CMAKE_SOURCE_DIR}/src/lib/win/libraylib.a 
        winmm gdi32 opengl32
    )
elseif(EMSCRIPTEN)
    target_link_libraries(dynamic_maze PRIVATE 
        ${CMAKE_SOURCE_DIR}/src/lib/libraylib.web.a
    )
    
    target_compile_definitions(dynamic_maze PRIVATE PLATFORM_WEB)
    
    set_target_properties(dynamic_maze PROPERTIES 
        OUTPUT_NAME "index"
        SUFFIX ".html"
        RUNTIME_OUTPUT_DIRECTORY "/home/zerotwo/Desktop/Code/Project/Project-2"
    )

    target_link_options(dynamic_maze PRIVATE
        "SHELL:-s USE_GLFW=3"
        "SHELL:-s WASM=1"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/../shell.html"
    )
else()
    find_package(raylib REQUIRED)
    
    if(TARGET raylib::raylib)
        target_link_libraries(dynamic_maze PRIVATE raylib::raylib)
    else()
        target_link_libraries(dynamic_maze PRIVATE raylib)
    endif()
    
    target_link_libraries(dynamic_maze PRIVATE GL m pthread dl rt X11)
endif()