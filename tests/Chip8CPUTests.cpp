#include <gtest/gtest.h>

#include "Chip8Core/Chip8CPU.h"
#include "Chip8Core/Chip8GraphicsBuffer.h"
#include "Chip8Core/Chip8Memory.h"

class Chip8CPUTest : public ::testing::Test
{
  protected:
    chip8core::Chip8Memory         memory;
    chip8core::Chip8GraphicsBuffer graphics;
    chip8core::Chip8InputBuffer    input;
    chip8core::Chip8Timer          delayTimer;
    chip8core::Chip8Timer          soundTimer;
    chip8core::Chip8CPU            cpu;

    Chip8CPUTest() : cpu(memory, graphics, input, delayTimer, soundTimer) {}
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

    for (int y = 0; y < chip8core::Chip8GraphicsBuffer::FRAMEBUFFER_HEIGHT; ++y)
    {
        for (int x = 0; x < chip8core::Chip8GraphicsBuffer::FRAMEBUFFER_WIDTH; ++x)
        {
            EXPECT_EQ(graphics.getPixel(x, y), 0)
                << "Pixel at (" << x << ", " << y << ") should be off";
        }
    }

    for (int i = 0; i < 16; ++i)
    {
        EXPECT_EQ(memory.read((i * 5) + 0x50), cpu.getFont(i * 5))
            << "Font data at index " << i << " should match";
    }
}

TEST_F(Chip8CPUTest, opcode_00E0_test)
{
    memory.write(0x200, 0x00);
    memory.write(0x201, 0xE0);

    graphics.setPixel(0, 0, 1);

    cpu.cycle();

    EXPECT_EQ(graphics.getPixel(0, 0), 0) << "Pixel should be off";
    EXPECT_EQ(cpu.getPC(), 0x202) << "PC should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_00EE_test)
{
    memory.write(0x200, 0x22);
    memory.write(0x201, 0x56);
    memory.write(0x256, 0x00);
    memory.write(0x257, 0xEE);

    cpu.cycle();
    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should return to 0x202 after RET";
    EXPECT_EQ(cpu.getSP(), 0) << "Stack pointer should be decremented to 0";
}

TEST_F(Chip8CPUTest, opcode_1NNN_test)
{
    memory.write(0x200, 0x12);
    memory.write(0x201, 0x34);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x234) << "Program counter should jump to 0x234";
}

TEST_F(Chip8CPUTest, opcode_2NNN_test)
{
    memory.write(0x200, 0x22);
    memory.write(0x201, 0x34);

    cpu.cycle();

    EXPECT_EQ(cpu.getSP(), 1) << "Stack pointer should be incremented to 1";
    EXPECT_EQ(cpu.getPC(), 0x234) << "Program counter should jump to 0x234";
}

TEST_F(Chip8CPUTest, opcode_3XNN_test_Equal)
{
    memory.write(0x200, 0x34);
    memory.write(0x201, 0x45);

    cpu.setV(4, 0x45);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_3XNN_test_Not_Equal)
{
    memory.write(0x200, 0x34);
    memory.write(0x201, 0x45);

    cpu.setV(4, 0x22);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should not skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_4XNN_test_Equal)
{
    memory.write(0x200, 0x44);
    memory.write(0x201, 0x45);

    cpu.setV(4, 0x45);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should not skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_4XNN_test_Not_Equal)
{
    memory.write(0x200, 0x44);
    memory.write(0x201, 0x45);

    cpu.setV(4, 0x22);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_5XY0_test_Equal)
{
    memory.write(0x200, 0x54);
    memory.write(0x201, 0x50);

    cpu.setV(4, 0x45);
    cpu.setV(5, 0x45);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_5XY0_test_Not_Equal)
{
    memory.write(0x200, 0x54);
    memory.write(0x201, 0x50);

    cpu.setV(4, 0x22);
    cpu.setV(5, 0x34);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should not skip next instruction";
}

TEST_F(Chip8CPUTest, opcode_6XNN_test)
{
    memory.write(0x200, 0x62);
    memory.write(0x201, 0xDE);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(2), 0xDE) << "V[2] should be equal to 0xDE";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_7XNN_test)
{
    memory.write(0x200, 0x73);
    memory.write(0x201, 0x25);

    cpu.setV(3, 0x4B);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(3), 0x70) << "V[3] should be equal to 0x70";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY0_test)
{
    memory.write(0x200, 0x80);
    memory.write(0x201, 0x20);

    cpu.setV(0, 0x42);
    cpu.setV(2, 0x99);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0), 0x99) << "V[0] should be equal to V[2]";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY1_test)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x21);

    cpu.setV(1, 0x42);
    cpu.setV(2, 0x99);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0xDB) << "V[1] should be equal to V[0] OR V[2]";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY2_test)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x22);

    cpu.setV(1, 0x42);
    cpu.setV(2, 0x99);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0x00) << "V[1] should be equal to V[1] AND V[2]";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY3_test)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x23);

    cpu.setV(1, 0x42);
    cpu.setV(2, 0x99);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0xDB) << "V[1] should be equal to V[1] XOR V[2]";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY4_test_Carry)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x24);

    cpu.setV(1, 0xFF);
    cpu.setV(2, 0x45);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0x44) << "V[1] should be equal to V[1] + V[2]";
    EXPECT_EQ(cpu.getV(0xF), 1) << "VF should be set to 1 (carry)";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY4_test_Carry_XisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x24);

    cpu.setV(0xF, 0xFF);
    cpu.setV(2, 0x45);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 1) << "VF should be set to 1 (carry)";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY4_test_No_Carry)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x24);

    cpu.setV(1, 0x20);
    cpu.setV(2, 0x30);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0x50) << "V[1] should be equal to V[1] + V[2]";
    EXPECT_EQ(cpu.getV(0xF), 0) << "VF should be set to 0 (no carry)";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY4_test_No_Carry_XisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x24);

    cpu.setV(0xF, 0x20);
    cpu.setV(2, 0x30);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 0) << "VF should be set to 0 (no carry)";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY5_NoBorrow)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x25);

    cpu.setV(1, 0x50);
    cpu.setV(2, 0x30);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0x20) << "V[1] should be equal to V[1] - V[2]";
    EXPECT_EQ(cpu.getV(0xF), 1) << "VF should be set to 1 (no borrow)";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY5_NoBorrow_XisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x25);

    cpu.setV(0xF, 0x50);
    cpu.setV(2, 0x30);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 1) << "VF should be set to 1 (no borrow)";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY5_Borrow)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x25);

    cpu.setV(1, 0x30);
    cpu.setV(2, 0x50);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0xE0) << "V[1] should be equal to V[1] - V[2]";
    EXPECT_EQ(cpu.getV(0xF), 0) << "VF should be set to 0 (borrow)";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY5_Borrow_XisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x25);

    cpu.setV(0xF, 0x30);
    cpu.setV(2, 0x50);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 0) << "VF should be set to 0 (borrow)";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY6_test_LSB_1)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x26);

    cpu.setV(1, 0b00100101);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0b00010010) << "V[1] should be equal to V[1] SHR 1";
    EXPECT_EQ(cpu.getV(0xF), 1) << "VF should be set to 1";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY6_test_LSB_1_xisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x26);

    cpu.setV(0xF, 0b00100101);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 1) << "VF should be set to 1";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY6_test_LSB_0)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x26);

    cpu.setV(1, 0b00010010);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0b00001001) << "V[1] should be equal to V[1] SHR 1";
    EXPECT_EQ(cpu.getV(0xF), 0) << "VF should be set to 0";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY6_test_LSB_0_xisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x26);

    cpu.setV(0xF, 0b00010010);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 0) << "VF should be set to 0";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY7_test_VY_Greater)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x27);

    cpu.setV(1, 0x10);
    cpu.setV(2, 0x30);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0x20) << "V[1] should be equal to 0x20";
    EXPECT_EQ(cpu.getV(0xF), 1) << "VF should be set to 1";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY7_test_VY_Greater_xisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x27);

    cpu.setV(0xF, 0x10);
    cpu.setV(2, 0x30);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 1) << "VF should be set to 1";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY7_test_VY_Less)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x27);

    cpu.setV(1, 0x30);
    cpu.setV(2, 0x10);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(1), 0xE0) << "V[1] should be equal to 0xE0";
    EXPECT_EQ(cpu.getV(0xF), 0) << "VF should be set to 0";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XY7_test_VY_Less_xisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x27);

    cpu.setV(0xF, 0x30);
    cpu.setV(2, 0x10);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 0) << "VF should be set to 0";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XYE_test_MSB_1)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x2E);

    cpu.setV(1, 0b10010010);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 1) << "V[F] should be equal to 0x01";
    EXPECT_EQ(cpu.getV(1), 0b00100100) << "V[1] should be equal to 0b00100100";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XYE_test_MSB_1_xisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x2E);

    cpu.setV(0x0F, 0b10010010);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 1) << "V[F] should be equal to 0x01";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XYE_test_MSB_0)
{
    memory.write(0x200, 0x81);
    memory.write(0x201, 0x2E);

    cpu.setV(1, 0b00100100);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 0) << "V[F] should be equal to 0x00";
    EXPECT_EQ(cpu.getV(1), 0b01001000) << "V[1] should be equal to 0b01001000";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_8XYE_test_MSB_0_xisF)
{
    memory.write(0x200, 0x8F);
    memory.write(0x201, 0x2E);

    cpu.setV(0x0F, 0b00100100);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0xF), 0) << "V[F] should be equal to 0x00";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_9XY0_test_NotEqual)
{
    memory.write(0x200, 0x91);
    memory.write(0x201, 0x20);

    cpu.setV(1, 0x22);
    cpu.setV(2, 0x24);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should be incremented by 4";
}

TEST_F(Chip8CPUTest, opcode_9XY0_test_Equal)
{
    memory.write(0x200, 0x91);
    memory.write(0x201, 0x20);

    cpu.setV(1, 0x22);
    cpu.setV(2, 0x22);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_ANNN_test)
{
    memory.write(0x200, 0xA1);
    memory.write(0x201, 0x23);

    cpu.cycle();

    EXPECT_EQ(cpu.getI(), 0x123) << "I should be equal to 0x123";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_BNNN_test)
{
    memory.write(0x200, 0xB2);
    memory.write(0x201, 0x23);

    cpu.setV(0, 0x10);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x233) << "Program counter should be 0x233 (NNN + V[0])";
}

TEST_F(Chip8CPUTest, opcode_CXKK_test)
{
    memory.write(0x200, 0xC1);
    memory.write(0x201, 0x2E);

    cpu.cycle();

    EXPECT_LE(cpu.getV(1), 0x2E) << "V[1] should be less than or equal to 0x2E after execution";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_DXYN_NoCollisionNoWrap)
{
    memory.write(0x200, 0xD1);
    memory.write(0x201, 0xA4);

    memory.write(0x300, 0b11111111);
    memory.write(0x301, 0b10000001);
    memory.write(0x302, 0b10000001);
    memory.write(0x303, 0b11111111);

    cpu.setI(0x300);
    cpu.setV(1, 5);
    cpu.setV(0xA, 1);

    cpu.cycle();

    EXPECT_EQ(graphics.getPixel(5, 1), 1) << "Pixel at (5, 1) should be set";
    EXPECT_EQ(graphics.getPixel(6, 1), 1) << "Pixel at (6, 1) should be set";
    EXPECT_EQ(graphics.getPixel(7, 1), 1) << "Pixel at (7, 1) should be set";
    EXPECT_EQ(graphics.getPixel(8, 1), 1) << "Pixel at (8, 1) should be set";
    EXPECT_EQ(graphics.getPixel(9, 1), 1) << "Pixel at (9, 1) should be set";
    EXPECT_EQ(graphics.getPixel(10, 1), 1) << "Pixel at (10, 1) should be set";
    EXPECT_EQ(graphics.getPixel(11, 1), 1) << "Pixel at (11, 1) should be set";
    EXPECT_EQ(graphics.getPixel(12, 1), 1) << "Pixel at (12, 1) should be set";

    EXPECT_EQ(graphics.getPixel(5, 2), 1) << "Pixel at (5, 2) should be set";
    EXPECT_EQ(graphics.getPixel(6, 2), 0) << "Pixel at (6, 2) should not be set";
    EXPECT_EQ(graphics.getPixel(7, 2), 0) << "Pixel at (7, 2) should not be set";
    EXPECT_EQ(graphics.getPixel(8, 2), 0) << "Pixel at (8, 2) should not be set";
    EXPECT_EQ(graphics.getPixel(9, 2), 0) << "Pixel at (9, 2) should not be set";
    EXPECT_EQ(graphics.getPixel(10, 2), 0) << "Pixel at (10, 2) should not be set";
    EXPECT_EQ(graphics.getPixel(11, 2), 0) << "Pixel at (11, 2) should not be set";
    EXPECT_EQ(graphics.getPixel(12, 2), 1) << "Pixel at (12, 2) should be set";

    EXPECT_EQ(graphics.getPixel(5, 3), 1) << "Pixel at (5, 3) should be set";
    EXPECT_EQ(graphics.getPixel(6, 3), 0) << "Pixel at (6, 3) should not be set";
    EXPECT_EQ(graphics.getPixel(7, 3), 0) << "Pixel at (7, 3) should not be set";
    EXPECT_EQ(graphics.getPixel(8, 3), 0) << "Pixel at (8, 3) should not be set";
    EXPECT_EQ(graphics.getPixel(9, 3), 0) << "Pixel at (9, 3) should not be set";
    EXPECT_EQ(graphics.getPixel(10, 3), 0) << "Pixel at (10, 3) should not be set";
    EXPECT_EQ(graphics.getPixel(11, 3), 0) << "Pixel at (11, 3) should not be set";
    EXPECT_EQ(graphics.getPixel(12, 3), 1) << "Pixel at (12, 3) should be set";

    EXPECT_EQ(graphics.getPixel(5, 4), 1) << "Pixel at (5, 4) should be set";
    EXPECT_EQ(graphics.getPixel(6, 4), 1) << "Pixel at (6, 4) should be set";
    EXPECT_EQ(graphics.getPixel(7, 4), 1) << "Pixel at (7, 4) should be set";
    EXPECT_EQ(graphics.getPixel(8, 4), 1) << "Pixel at (8, 4) should be set";
    EXPECT_EQ(graphics.getPixel(9, 4), 1) << "Pixel at (9, 4) should be set";
    EXPECT_EQ(graphics.getPixel(10, 4), 1) << "Pixel at (10, 4) should be set";
    EXPECT_EQ(graphics.getPixel(11, 4), 1) << "Pixel at (11, 4) should be set";
    EXPECT_EQ(graphics.getPixel(12, 4), 1) << "Pixel at (12, 4) should be set";

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_DXYN_Collision)
{
    memory.write(0x200, 0xD1);
    memory.write(0x201, 0xA4);
    memory.write(0x202, 0xD2);
    memory.write(0x203, 0x34);

    memory.write(0x300, 0b11111111);
    memory.write(0x301, 0b10000001);
    memory.write(0x302, 0b10000001);
    memory.write(0x303, 0b11111111);

    cpu.setI(0x300);
    cpu.setV(1, 5);
    cpu.setV(0xA, 1);
    cpu.setV(2, 12);
    cpu.setV(3, 4);

    cpu.cycle();
    cpu.cycle();

    EXPECT_EQ(graphics.getPixel(11, 4), 1) << "Pixel at (11, 4) should be set";
    EXPECT_EQ(graphics.getPixel(12, 4), 0) << "Pixel at (12, 4) should not be set";
    EXPECT_EQ(graphics.getPixel(13, 4), 1) << "Pixel at (13, 4) should be set";

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_DXYN_Wrap)
{
    memory.write(0x200, 0xD1);
    memory.write(0x201, 0xA4);
    memory.write(0x202, 0xD2);
    memory.write(0x203, 0x34);

    memory.write(0x300, 0b11111111);
    memory.write(0x301, 0b10000001);
    memory.write(0x302, 0b10000001);
    memory.write(0x303, 0b11111111);

    cpu.setI(0x300);
    cpu.setV(1, 60);
    cpu.setV(0xA, 1);
    cpu.setV(2, 10);
    cpu.setV(3, 30);

    cpu.cycle();
    cpu.cycle();

    EXPECT_EQ(graphics.getPixel(1, 1), 1) << "Pixel at (1, 1) should be set";
    EXPECT_EQ(graphics.getPixel(63, 1), 1) << "Pixel at (63, 1) should be set";

    EXPECT_EQ(graphics.getPixel(10, 0), 1) << "Pixel at (10, 0) should be set";
    EXPECT_EQ(graphics.getPixel(10, 31), 1) << "Pixel at (10, 31) should be set";

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_EX9E_test_KeyPressed)
{
    memory.write(0x200, 0xE9);
    memory.write(0x201, 0x9E);

    input.setKeyState(0x01, true); // Simulate key 0x01 pressed
    cpu.setV(0x9, 0x01);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_EX9E_test_KeyNotPressed)
{
    memory.write(0x200, 0xE9);
    memory.write(0x201, 0x9E);

    input.setKeyState(0x01, false); // Simulate key 0x01 not pressed
    cpu.setV(0x9, 0x01);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should not be incremented";
}

TEST_F(Chip8CPUTest, opcode_EXA1_test_KeyNotPressed)
{
    memory.write(0x200, 0xE9);
    memory.write(0x201, 0xA1);

    input.setKeyState(0x01, false); // Simulate key 0x01 not pressed
    cpu.setV(0x9, 0x01);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x204) << "Program counter should be incremented";
}

TEST_F(Chip8CPUTest, opcode_EXA1_test_KeyPressed)
{
    memory.write(0x200, 0xE9);
    memory.write(0x201, 0xA1);

    input.setKeyState(0x01, true); // Simulate key 0x01 pressed
    cpu.setV(0x9, 0x01);

    cpu.cycle();

    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should not be incremented";
}

TEST_F(Chip8CPUTest, opcode_FX07_test)
{
    memory.write(0x200, 0xF0);
    memory.write(0x201, 0x07);

    cpu.setV(0x00, 0x22);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0x00), 0x00) << "V[0] should be equal to 0x00";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_FX0A_WaitForKey_NoKeyPressed)
{
    memory.write(0x200, 0xF1);
    memory.write(0x201, 0x0A);

    cpu.setV(1, 0x00);

    // No key is pressed
    for (int k = 0; k < 16; ++k)
        input.setKeyState(k, false);

    cpu.cycle();

    // PC should not advance, V1 should not change
    EXPECT_EQ(cpu.getPC(), 0x200);
    EXPECT_EQ(cpu.getV(1), 0x00);
}

TEST_F(Chip8CPUTest, opcode_FX15_test)
{
    memory.write(0x200, 0xF1);
    memory.write(0x201, 0x15);

    cpu.setV(0x01, 0x22);

    cpu.cycle();

    EXPECT_EQ(delayTimer.getValue(), 0x22) << "Delay timer should be equal to 0x22";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_FX18_test)
{
    memory.write(0x200, 0xF1);
    memory.write(0x201, 0x18);

    cpu.setV(0x01, 0x22);

    cpu.cycle();

    EXPECT_EQ(soundTimer.getValue(), 0x22) << "Sound timer should be equal to 0x22";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_FX1E_test)
{
    memory.write(0x200, 0xF2);
    memory.write(0x201, 0x1E);

    cpu.setV(2, 0x10);
    cpu.setI(0x100);

    cpu.cycle();

    EXPECT_EQ(cpu.getI(), 0x110) << "I should be equal to 0x110 (I + V[2])";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_FX29_test)
{
    memory.write(0x200, 0xF2);
    memory.write(0x201, 0x29);

    for (int i = 0; i < 16; ++i)
    {
        cpu.reset();
        cpu.setV(2, i);
        cpu.cycle();
        EXPECT_EQ(cpu.getI(), 0x50 + (i * 5)) << "I should be equal to " << (0x50 + (i * 5))
                                              << " (location of sprite for digit " << i << ")";
    }
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_FX33_test)
{
    memory.write(0x200, 0xF2);
    memory.write(0x201, 0x33);

    cpu.setV(2, 0xAB);
    cpu.setI(0x100);

    cpu.cycle();

    EXPECT_EQ(memory.read(0x100), 0x01) << "Memory at 0x100 should be equal to 0x01";
    EXPECT_EQ(memory.read(0x101), 0x07) << "Memory at 0x101 should be equal to 0x07";
    EXPECT_EQ(memory.read(0x102), 0x01) << "Memory at 0x102 should be equal to 0x01";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_FX55_test)
{
    memory.write(0x200, 0xFF);
    memory.write(0x201, 0x55);

    cpu.setV(0x0, 0x12);
    cpu.setV(0x1, 0x34);
    cpu.setV(0x2, 0x56);
    cpu.setV(0x3, 0x78);
    cpu.setV(0x4, 0x9A);
    cpu.setV(0x5, 0xBC);
    cpu.setV(0x6, 0xDE);
    cpu.setV(0x7, 0xF0);
    cpu.setV(0x8, 0x11);
    cpu.setV(0x9, 0x22);
    cpu.setV(0xA, 0x33);
    cpu.setV(0xB, 0x44);
    cpu.setV(0xC, 0x55);
    cpu.setV(0xD, 0x66);
    cpu.setV(0xE, 0x77);
    cpu.setV(0xF, 0x88);

    cpu.setI(0x100);

    cpu.cycle();

    EXPECT_EQ(memory.read(0x100), 0x12) << "Memory at 0x100 should be equal to 0x12";
    EXPECT_EQ(memory.read(0x101), 0x34) << "Memory at 0x101 should be equal to 0x34";
    EXPECT_EQ(memory.read(0x102), 0x56) << "Memory at 0x102 should be equal to 0x56";
    EXPECT_EQ(memory.read(0x103), 0x78) << "Memory at 0x103 should be equal to 0x78";
    EXPECT_EQ(memory.read(0x104), 0x9A) << "Memory at 0x104 should be equal to 0x9A";
    EXPECT_EQ(memory.read(0x105), 0xBC) << "Memory at 0x105 should be equal to 0xBC";
    EXPECT_EQ(memory.read(0x106), 0xDE) << "Memory at 0x106 should be equal to 0xDE";
    EXPECT_EQ(memory.read(0x107), 0xF0) << "Memory at 0x107 should be equal to 0xF0";
    EXPECT_EQ(memory.read(0x108), 0x11) << "Memory at 0x108 should be equal to 0x11";
    EXPECT_EQ(memory.read(0x109), 0x22) << "Memory at 0x109 should be equal to 0x22";
    EXPECT_EQ(memory.read(0x10A), 0x33) << "Memory at 0x10A should be equal to 0x33";
    EXPECT_EQ(memory.read(0x10B), 0x44) << "Memory at 0x10B should be equal to 0x44";
    EXPECT_EQ(memory.read(0x10C), 0x55) << "Memory at 0x10C should be equal to 0x55";
    EXPECT_EQ(memory.read(0x10D), 0x66) << "Memory at 0x10D should be equal to 0x66";
    EXPECT_EQ(memory.read(0x10E), 0x77) << "Memory at 0x10E should be equal to 0x77";
    EXPECT_EQ(memory.read(0x10F), 0x88) << "Memory at 0x10F should be equal to 0x88";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

TEST_F(Chip8CPUTest, opcode_FX65_test)
{
    memory.write(0x200, 0xFF);
    memory.write(0x201, 0x65);

    memory.write(0x100, 0x12);
    memory.write(0x101, 0x34);
    memory.write(0x102, 0x56);
    memory.write(0x103, 0x78);
    memory.write(0x104, 0x9A);
    memory.write(0x105, 0xBC);
    memory.write(0x106, 0xDE);
    memory.write(0x107, 0xF0);
    memory.write(0x108, 0x11);
    memory.write(0x109, 0x22);
    memory.write(0x10A, 0x33);
    memory.write(0x10B, 0x44);
    memory.write(0x10C, 0x55);
    memory.write(0x10D, 0x66);
    memory.write(0x10E, 0x77);
    memory.write(0x10F, 0x88);

    cpu.setI(0x100);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(0), 0x12) << "V[0] should be equal to 0x12";
    EXPECT_EQ(cpu.getV(1), 0x34) << "V[1] should be equal to 0x34";
    EXPECT_EQ(cpu.getV(2), 0x56) << "V[2] should be equal to 0x56";
    EXPECT_EQ(cpu.getV(3), 0x78) << "V[3] should be equal to 0x78";
    EXPECT_EQ(cpu.getV(4), 0x9A) << "V[4] should be equal to 0x9A";
    EXPECT_EQ(cpu.getV(5), 0xBC) << "V[5] should be equal to 0xBC";
    EXPECT_EQ(cpu.getV(6), 0xDE) << "V[6] should be equal to 0xDE";
    EXPECT_EQ(cpu.getV(7), 0xF0) << "V[7] should be equal to 0xF0";
    EXPECT_EQ(cpu.getV(8), 0x11) << "V[8] should be equal to 0x11";
    EXPECT_EQ(cpu.getV(9), 0x22) << "V[9] should be equal to 0x22";
    EXPECT_EQ(cpu.getV(10), 0x33) << "V[10] should be equal to 0x33";
    EXPECT_EQ(cpu.getV(11), 0x44) << "V[11] should be equal to 0x44";
    EXPECT_EQ(cpu.getV(12), 0x55) << "V[12] should be equal to 0x55";
    EXPECT_EQ(cpu.getV(13), 0x66) << "V[13] should be equal to 0x66";
    EXPECT_EQ(cpu.getV(14), 0x77) << "V[14] should be equal to 0x77";
    EXPECT_EQ(cpu.getV(15), 0x88) << "V[15] should be equal to 0x88";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}