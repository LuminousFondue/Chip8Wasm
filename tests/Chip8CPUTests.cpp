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

TEST_F(Chip8CPUTest, opcode_00E0_test)
{
    // Turn on one pixel
    graphics.setPixel(0, 0, 1);
    EXPECT_EQ(graphics.getPixel(0, 0), 1) << "Pixel should be on";

    memory.write(0x200, 0x00);
    memory.write(0x201, 0xE0);

    // Execute the cycle
    cpu.cycle();
    EXPECT_EQ(graphics.getPixel(0, 0), 0) << "Pixel should be off";
    EXPECT_EQ(cpu.getPC(), 0x202) << "PC should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_00EE_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_1NNN_test)
{
    EXPECT_EQ(cpu.getPC(), 0x200) << "Initial program counter should be 0x200";

    memory.write(0x200, 0x12);
    memory.write(0x201, 0x34);

    // Execute the cycle
    cpu.cycle();

    // Check if the program counter was set correctly
    EXPECT_EQ(cpu.getPC(), 0x234) << "Program counter should jump to 0x234";
}

TEST_F(Chip8CPUTest, opcode_2NNN_test)
{
    EXPECT_EQ(cpu.getSP(), 0) << "Initial stack pointer should be 0";

    memory.write(0x200, 0x22);
    memory.write(0x201, 0x34);

    cpu.cycle();

    // Check if the stack pointer was incremented and PC set correctly
    EXPECT_EQ(cpu.getSP(), 1) << "Stack pointer should be incremented to 1";
    EXPECT_EQ(cpu.getPC(), 0x234) << "Program counter should jump to 0x234";
}

TEST_F(Chip8CPUTest, opcode_3XNN_test)
{
    memory.write(0x200, 0x34);
    memory.write(0x201, 0x45);
    cpu.setV(4, 0x45);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should skip next instruction";

    cpu.reset();
    cpu.setV(4, 0x22);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should not skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_4XNN_test)
{
    memory.write(0x200, 0x44);
    memory.write(0x201, 0x45);
    cpu.setV(4, 0x45);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should not skip next instruction";

    cpu.reset();
    cpu.setV(4, 0x22);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_5XY0_test)
{
    memory.write(0x200, 0x54);
    memory.write(0x201, 0x50);
    cpu.setV(4, 0x45);
    cpu.setV(5, 0x45);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should skip next instruction";

    cpu.reset();
    cpu.setV(4, 0x22);
    cpu.setV(5, 0x34);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should not skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_6XNN_test)
{
    EXPECT_EQ(cpu.getV(2), 0) << "V[2] should be equal to 0";

    memory.write(0x200, 0x62);
    memory.write(0x201, 0xDE);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(2), 0xDE) << "V[2] should be equal to 0xDE";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_7XNN_test)
{
    EXPECT_EQ(cpu.getV(3), 0) << "V[3] should be equal to 0";

    cpu.setV(3, 0x4B);

    EXPECT_EQ(cpu.getV(3), 0x4B) << "V[3] should be equal to 0x4B";

    memory.write(0x200, 0x73);
    memory.write(0x201, 0x25);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(3), 0x70) << "V[3] should be equal to 0x70";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

/**
 * 8XY0: LD Vx, Vy - Set Vx = Vy
 *
 * Stores the value of register Vy in register Vx.
 */
TEST_F(Chip8CPUTest, opcode_8XY0_test)
{
    cpu.setV(0, 0x42);
    cpu.setV(2, 0x99);

    EXPECT_EQ(cpu.getV(0), 0x42) << "V[0] should be equal to 0x42";
    EXPECT_EQ(cpu.getV(2), 0x99) << "V[2] should be equal to 0x99";

    memory.write(0x200, 0x80);
    memory.write(0x201, 0x20);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0), 0x99) << "V[0] should be equal to V[2]";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY1_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_8XY2_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_8XY3_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_8XY4_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_8XY5_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_8XY6_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_8XY7_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_8XYE_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_9XY0_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_ANNN_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_BNNN_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_CXNN_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_DXYN_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_EX9E_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_EXA1_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX07_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX0A_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX15_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX18_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX1E_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX29_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX33_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX55_test)
{
    FAIL() << "Not yet implemented";
}

TEST_F(Chip8CPUTest, opcode_FX65_test)
{
    FAIL() << "Not yet implemented";
}