cmake_minimum_required(VERSION 3.10.0)
project(Chip8Wasm VERSION 0.1.0 LANGUAGES C CXX)

# -----------------------------------------------------------------------------
# Compiler Settings
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Compiler Options
# -----------------------------------------------------------------------------
option(BUILD_EMULATOR "Build the native Chip8 emulator executable" ON)
option(BUILD_WASM "Build the WASM Chip8 emulator executable" ON)
option(BUILD_TESTS "Build unit tests" ON)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
include(FetchContent)

# SPDLOG - C++ Logging Library
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(spdlog)

# GoogleTest - C++ Testing Framework
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(BUILD_EMULATOR)
    # SDL 2
    find_package(SDL2 REQUIRED)
endif()

# -----------------------------------------------------------------------------
# Core Library
# -----------------------------------------------------------------------------
add_library(Chip8Core STATIC
    src/Chip8Core/Chip8.cpp
    src/Chip8Core/Chip8Memory.cpp
    src/Chip8Core/Chip8CPU.cpp
    src/Chip8Core/Chip8GraphicsBuffer.cpp
    src/Chip8Core/Chip8InputBuffer.cpp
    src/Chip8Core/Chip8Timer.cpp
)
target_include_directories(Chip8Core PRIVATE include)
target_link_libraries(Chip8Core PRIVATE spdlog::spdlog)

# -----------------------------------------------------------------------------
# Main Executable
# -----------------------------------------------------------------------------
if(BUILD_EMULATOR)
    add_executable(
        Chip8Emulator
        src/Chip8Emulator/main.cpp
        src/Chip8Emulator/Chip8Display.cpp
        src/Chip8Emulator/Chip8Input.cpp
        src/Chip8Emulator/Chip8ROMLoader.cpp
        src/Chip8Emulator/Chip8Audio.cpp
    )

    target_link_libraries(
        Chip8Emulator PRIVATE
        spdlog::spdlog
        Chip8Core
        SDL2::SDL2
    )
    target_include_directories(Chip8Emulator PRIVATE include)
endif()

# -----------------------------------------------------------------------------
# WASM
# -----------------------------------------------------------------------------
if(BUILD_WASM)
    add_executable(
        Chip8Wasm
        src/Chip8Wasm/main.cpp
    )

    target_link_libraries(
        Chip8Wasm PRIVATE
        spdlog::spdlog
        Chip8Core
    )
    target_include_directories(Chip8Wasm PRIVATE include)
    set_target_properties(Chip8Wasm PROPERTIES SUFFIX ".html")
    target_link_options(Chip8Wasm PRIVATE --bind -sUSE_SDL=2 -oChip8Wasm.html)
endif()

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
if(BUILD_TESTS)
    include(CTest)
    enable_testing()

    add_executable(
        Chip8Tests
        tests/Chip8MemoryTests.cpp
        tests/Chip8GraphicsBufferTests.cpp
        tests/Chip8CPUTests.cpp
    )
    target_compile_definitions(Chip8Tests PRIVATE UNIT_TEST)

    target_include_directories(Chip8Tests PRIVATE include)
    target_link_libraries(Chip8Tests PRIVATE GTest::gtest_main Chip8Core)

    include(GoogleTest)
    gtest_discover_tests(Chip8Tests)
endif()

# -----------------------------------------------------------------------------
# Documentation (Doxygen)
# -----------------------------------------------------------------------------
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()