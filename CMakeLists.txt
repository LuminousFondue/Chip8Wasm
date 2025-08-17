cmake_minimum_required(VERSION 3.10.0)
project(Chip8Wasm VERSION 0.1.0 LANGUAGES C CXX)

# Dependencies
## SPDLOG - C++ Logging Library
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0 # Use the latest stable tag or your preferred version
)
FetchContent_MakeAvailable(spdlog)

# Testing Framework
## GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
## For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    Chip8Tests
    tests/PlaceholderTest.cpp
)

target_include_directories(
    Chip8Tests PRIVATE 
    include
)

target_link_libraries(
    Chip8Tests
    GTest::gtest_main
    spdlog::spdlog
)

include(GoogleTest)
gtest_discover_tests(Chip8Tests)

# Build
add_executable(
    Chip8Wasm
    src/main.cpp
    src/Chip8.cpp
    src/Chip8CPU.cpp
    src/Chip8Memory.cpp
)

# Include directories and link libraries
target_include_directories(
    Chip8Wasm PRIVATE 
    include
)

target_link_libraries(
    Chip8Wasm PRIVATE
    spdlog::spdlog
)

# Install
install(TARGETS Chip8Wasm DESTINATION bin)

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()