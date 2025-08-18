#include <gtest/gtest.h>

#include "Chip8CPU.h"
#include "Chip8Memory.h"

TEST(Chip8CPUTests, Initialization_Reset)
{
    Chip8Memory memory;
    Chip8CPU    cpu(memory);

    // Check initial state of CPU
    EXPECT_EQ(cpu.getPC(), 0x200); // Program counter should start at 0x200
    EXPECT_EQ(cpu.getSP(), 0);     // Stack pointer should start at 0
    EXPECT_EQ(cpu.getI(), 0);      // Index register should start at 0

    for (int i = 0; i < 16; ++i)
    {
        EXPECT_EQ(cpu.getV(i), 0); // All registers should be initialized to 0
    }
}