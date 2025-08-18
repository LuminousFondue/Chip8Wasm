#include <gtest/gtest.h>

#include "Chip8CPU.h"
#include "Chip8GraphicsData.h"
#include "Chip8Memory.h"

class Chip8CPUTest : public ::testing::Test
{
  protected:
    Chip8Memory       memory;
    Chip8GraphicsData graphics;
    Chip8CPU          cpu;

    Chip8CPUTest() : cpu(memory, graphics) {}
};

TEST_F(Chip8CPUTest, Initialization_Reset)
{
    EXPECT_EQ(cpu.getPC(), 0x200);
    EXPECT_EQ(cpu.getSP(), 0);
    EXPECT_EQ(cpu.getI(), 0);
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_EQ(cpu.getV(i), 0);
    }
}