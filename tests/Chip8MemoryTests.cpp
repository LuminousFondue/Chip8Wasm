#include <gtest/gtest.h>

#include "Chip8Memory.h"

// Test that writing and reading a valid address works.
TEST(Chip8MemoryTest, WriteAndReadValidAddress)
{
    Chip8Memory mem;
    mem.write(100, 0xAB);
    EXPECT_EQ(mem.read(100), 0xAB);
}

// Test that reading out of bounds throws an exception.
TEST(Chip8MemoryTest, ReadOutOfBoundsThrows)
{
    Chip8Memory mem;
    EXPECT_THROW(mem.read(Chip8Memory::MEMORY_SIZE), Chip8MemoryException);
}

// Test that writing out of bounds throws an exception.
TEST(Chip8MemoryTest, WriteOutOfBoundsThrows)
{
    Chip8Memory mem;
    EXPECT_THROW(mem.write(Chip8Memory::MEMORY_SIZE, 0x12), Chip8MemoryException);
}

// Test that clearing the memory sets all bytes to zero.
TEST(Chip8MemoryTest, ClearMemorySetsAllZero)
{
    Chip8Memory mem;

    // Fill memory with non-zero values
    for (uint16_t i = 0; i < Chip8Memory::MEMORY_SIZE; ++i)
    {
        mem.write(i, 0xAB);
    }

    // Verify that all memory is non-zero
    for (uint16_t i = 0; i < Chip8Memory::MEMORY_SIZE; ++i)
    {
        EXPECT_NE(mem.read(i), 0);
    }

    mem.clear();

    // Check that all memory is zero
    for (uint16_t i = 0; i < Chip8Memory::MEMORY_SIZE; ++i)
    {
        EXPECT_EQ(mem.read(i), 0);
    }
}