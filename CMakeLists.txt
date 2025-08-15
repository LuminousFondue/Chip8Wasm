cmake_minimum_required(VERSION 3.10.0)
project(Chip8Wasm VERSION 0.1.0 LANGUAGES C CXX)

# GoogleTest integration
# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


# Test
add_executable(
    Chip8Tests
    tests/PlaceholderTest.cpp
)

target_link_libraries(
    Chip8Tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Chip8Tests)


# Build
add_executable(
    Chip8Wasm
    src/main.cpp
)


# Install
install(TARGETS Chip8Wasm DESTINATION bin)