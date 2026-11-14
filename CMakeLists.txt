cmake_minimum_required(VERSION 3.16.3)

project(ParticleSimulation
    VERSION 1.0.0
    DESCRIPTION "High-performance C++ Particle Simulation Library"
    LANGUAGES CXX
)

# Enforce standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for language servers and tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build configuration options
option(BUILD_DEMOS "Build simulation demonstration executables" ON)
option(BUILD_TESTING "Build GoogleTest test suite" ON)
option(BUILD_DOCS "Enable Doxygen documentation target" ON)
option(ENABLE_PROFILING "Enable compilation with profiling flag -pg" OFF)

# Warning flags and compiler options
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra)
    if(ENABLE_PROFILING)
        add_compile_options(-pg)
        add_link_options(-pg)
    endif()
endif()

# Header search paths
include_directories(${CMAKE_SOURCE_DIR}/include)

# Core library
add_subdirectory(src)

# Demonstration executables
if(BUILD_DEMOS)
    add_subdirectory(demo)
endif()

# Documentation target (Doxygen)
if(BUILD_DOCS)
    add_subdirectory(doc)
endif()

# Test suite (GoogleTest / CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.8.0
    )
    # Prevent overriding parent settings on Windows platforms
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(test)
endif()
