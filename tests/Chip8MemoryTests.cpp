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

// Test writing and reading a block of data at a valid address.
TEST(Chip8MemoryTest, WriteAndReadBlockValid)
{
    Chip8Memory          mem;
    std::vector<uint8_t> data    = {0x01, 0x02, 0x03, 0x04};
    uint16_t             address = 200;
    mem.write(address, data);

    std::vector<uint8_t> readData = mem.read(address, data.size());
    EXPECT_EQ(readData, data);
}

// Test writing a block that would overflow memory throws an exception.
TEST(Chip8MemoryTest, WriteBlockOutOfBoundsThrows)
{
    Chip8Memory          mem;
    std::vector<uint8_t> data(10, 0xFF);
    // Address chosen so that address + data.size() > MEMORY_SIZE
    uint16_t address = Chip8Memory::MEMORY_SIZE - 5;
    EXPECT_THROW(mem.write(address, data), Chip8MemoryException);
}

// Test reading a block that would overflow memory throws an exception.
TEST(Chip8MemoryTest, ReadBlockOutOfBoundsThrows)
{
    Chip8Memory mem;
    size_t      length  = 10;
    uint16_t    address = Chip8Memory::MEMORY_SIZE - 5;
    EXPECT_THROW(mem.read(address, length), Chip8MemoryException);
}

// Test writing and reading a block at the very end of memory.
TEST(Chip8MemoryTest, WriteAndReadBlockAtEnd)
{
    Chip8Memory          mem;
    std::vector<uint8_t> data    = {0xAA, 0xBB, 0xCC};
    uint16_t             address = Chip8Memory::MEMORY_SIZE - data.size();
    mem.write(address, data);

    std::vector<uint8_t> readData = mem.read(address, data.size());
    EXPECT_EQ(readData, data);
}

// Test writing a zero-length block does not throw and does nothing.
TEST(Chip8MemoryTest, WriteZeroLengthBlock)
{
    Chip8Memory          mem;
    std::vector<uint8_t> data;
    uint16_t             address = 50;
    EXPECT_NO_THROW(mem.write(address, data));
    // Memory should remain unchanged (still zero)
    EXPECT_EQ(mem.read(address), 0);
}

// Test reading a zero-length block returns an empty vector.
TEST(Chip8MemoryTest, ReadZeroLengthBlock)
{
    Chip8Memory          mem;
    uint16_t             address = 100;
    std::vector<uint8_t> result  = mem.read(address, 0);
    EXPECT_TRUE(result.empty());
}

// Test that dump returns the entire memory contents as a vector.
TEST(Chip8MemoryTest, DumpReturnsFullMemory)
{
    Chip8Memory mem;

    // Fill memory with a pattern
    for (uint16_t i = 0; i < Chip8Memory::MEMORY_SIZE; ++i)
    {
        mem.write(i, static_cast<uint8_t>(i & 0xFF));
    }

    std::vector<uint8_t> dumped = mem.dump();

    ASSERT_EQ(dumped.size(), Chip8Memory::MEMORY_SIZE);

    for (uint16_t i = 0; i < Chip8Memory::MEMORY_SIZE; ++i)
    {
        EXPECT_EQ(dumped[i], static_cast<uint8_t>(i & 0xFF));
    }
}

// Test that dump reflects changes after clear.
TEST(Chip8MemoryTest, DumpAfterClearIsZero)
{
    Chip8Memory mem;

    // Fill memory with non-zero values
    for (uint16_t i = 0; i < Chip8Memory::MEMORY_SIZE; ++i)
    {
        mem.write(i, 0xAA);
    }

    mem.clear();
    std::vector<uint8_t> dumped = mem.dump();

    ASSERT_EQ(dumped.size(), Chip8Memory::MEMORY_SIZE);

    for (uint8_t value : dumped)
    {
        EXPECT_EQ(value, 0);
    }
}
