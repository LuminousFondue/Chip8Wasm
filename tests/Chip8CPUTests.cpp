#include <gtest/gtest.h>

#include "Chip8CPU.h"
#include "Chip8Memory.h"

class Chip8CPUTest : public ::testing::Test
{
  protected:
    Chip8Memory memory_;
    Chip8CPU    cpu_;

    Chip8CPUTest() : cpu_(memory_) {}
};

// Tests that Chip8CPU is created with proper initialized values
TEST_F(Chip8CPUTest, Initialization)
{
    // Check that the CPU registers are initialized to their default values
    EXPECT_EQ(cpu_.getPC(), 0x200);
    EXPECT_EQ(cpu_.getSP(), 0);
    EXPECT_EQ(cpu_.getI(), 0);

    // Check that the stack is empty
    EXPECT_EQ(cpu_.getSP(), 0);
}

// Tests that Chip8CPU can be reset to its initial state
TEST_F(Chip8CPUTest, Reset)
{
    // Modify the CPU registers
    cpu_.PC() = 0x1234;
    cpu_.SP() = 0x000F;
    cpu_.I()  = 0x00FF;

    for (int i = 0; i < 16; ++i)
    {
        cpu_.V()[i] = i;
    }

    cpu_.reset();

    EXPECT_EQ(cpu_.getPC(), 0x200);
    EXPECT_EQ(cpu_.getSP(), 0);
    EXPECT_EQ(cpu_.getI(), 0);
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_EQ(cpu_.V()[i], 0);
    }
}