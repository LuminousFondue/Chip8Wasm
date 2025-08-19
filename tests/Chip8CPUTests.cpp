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

/**
 * @brief Test the 00E0 opcode (CLS - Clear the display).
 *
 * This test verifies that executing the 00E0 opcode clears all pixels in the graphics buffer.
 */
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

/**
 * @brief Test the 00EE opcode (RET - Return from subroutine).
 *
 * This test verifies that executing the 00EE opcode pops the return address from the stack
 * and sets the program counter to that address.
 */
TEST_F(Chip8CPUTest, opcode_00EE_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 1NNN opcode (JP - Jump to address).
 *
 * This test verifies that executing the 1NNN opcode sets the program counter to the specified
 * address.
 */
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

/**
 * @brief Test the 2NNN opcode (CALL - Call subroutine at NNN).
 *
 * This test verifies that executing the 2NNN opcode increments the stack pointer,
 * stores the current program counter on the stack, and sets the program counter to the subroutine
 * address.
 */
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

/**
 * @brief Test the 3XNN opcode (SE Vx, NN - Skip next instruction if Vx == NN).
 *
 * This test verifies that executing the 3XNN opcode skips the next instruction if Vx equals NN,
 * and does not skip if Vx does not equal NN.
 */
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
/**
 * @brief Test the 4XNN opcode (SNE Vx, NN - Skip next instruction if Vx != NN).
 *
 * This test verifies that executing the 4XNN opcode skips the next instruction if Vx does not equal
 * NN, and does not skip if Vx equals NN.
 */
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

/**
 * @brief Test the 5XY0 opcode (SE Vx, Vy - Skip next instruction if Vx == Vy).
 *
 * This test verifies that executing the 5XY0 opcode skips the next instruction if Vx equals Vy,
 * and does not skip if Vx does not equal Vy.
 */
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

/**
 * @brief Test the 6XNN opcode (LD Vx, NN - Set Vx = NN).
 *
 * This test verifies that executing the 6XNN opcode sets register Vx to the specified value NN.
 */
TEST_F(Chip8CPUTest, opcode_6XNN_test)
{
    EXPECT_EQ(cpu.getV(2), 0) << "V[2] should be equal to 0";

    memory.write(0x200, 0x62);
    memory.write(0x201, 0xDE);

    cpu.cycle();

    EXPECT_EQ(cpu.getV(2), 0xDE) << "V[2] should be equal to 0xDE";
    EXPECT_EQ(cpu.getPC(), 0x202) << "Program counter should be incremented by 2";
}

/**
 * @brief Test the 7XNN opcode (ADD Vx, NN - Set Vx = Vx + NN).
 *
 * This test verifies that executing the 7XNN opcode adds NN to register Vx.
 */
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
 * @brief Test the 8XY0 opcode (LD Vx, Vy - Set Vx = Vy).
 *
 * This test verifies that executing the 8XY0 opcode sets register Vx to the value of Vy.
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

/**
 * @brief Test the 8XY1 opcode (OR Vx, Vy - Set Vx = Vx OR Vy).
 *
 * This test verifies that executing the 8XY1 opcode sets Vx to the bitwise OR of Vx and Vy.
 */
TEST_F(Chip8CPUTest, opcode_8XY1_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 8XY2 opcode (AND Vx, Vy - Set Vx = Vx AND Vy).
 *
 * This test verifies that executing the 8XY2 opcode sets Vx to the bitwise AND of Vx and Vy.
 */
TEST_F(Chip8CPUTest, opcode_8XY2_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 8XY3 opcode (XOR Vx, Vy - Set Vx = Vx XOR Vy).
 *
 * This test verifies that executing the 8XY3 opcode sets Vx to the bitwise XOR of Vx and Vy.
 */
TEST_F(Chip8CPUTest, opcode_8XY3_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 8XY4 opcode (ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry).
 *
 * This test verifies that executing the 8XY4 opcode adds Vy to Vx and sets VF if there is a carry.
 */
TEST_F(Chip8CPUTest, opcode_8XY4_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 8XY5 opcode (SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow).
 *
 * This test verifies that executing the 8XY5 opcode subtracts Vy from Vx and sets VF if there is no
 * borrow.
 */
TEST_F(Chip8CPUTest, opcode_8XY5_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 8XY6 opcode (SHR Vx {, Vy} - Set Vx = Vx SHR 1).
 *
 * This test verifies that executing the 8XY6 opcode shifts Vx right by one and sets VF to the least
 * significant bit before the shift.
 */
TEST_F(Chip8CPUTest, opcode_8XY6_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 8XY7 opcode (SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow).
 *
 * This test verifies that executing the 8XY7 opcode sets Vx to Vy minus Vx and sets VF if there is
 * no borrow.
 */
TEST_F(Chip8CPUTest, opcode_8XY7_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 8XYE opcode (SHL Vx {, Vy} - Set Vx = Vx SHL 1).
 *
 * This test verifies that executing the 8XYE opcode shifts Vx left by one and sets VF to the most
 * significant bit before the shift.
 */
TEST_F(Chip8CPUTest, opcode_8XYE_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the 9XY0 opcode (SNE Vx, Vy - Skip next instruction if Vx != Vy).
 *
 * This test verifies that executing the 9XY0 opcode skips the next instruction if Vx does not equal
 * Vy.
 */
TEST_F(Chip8CPUTest, opcode_9XY0_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the ANNN opcode (LD I, addr - Set I = NNN).
 *
 * This test verifies that executing the ANNN opcode sets the index register I to the specified
 * address.
 */
TEST_F(Chip8CPUTest, opcode_ANNN_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the BNNN opcode (JP V0, addr - Jump to address NNN + V0).
 *
 * This test verifies that executing the BNNN opcode sets the program counter to NNN plus V0.
 */
TEST_F(Chip8CPUTest, opcode_BNNN_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the CXNN opcode (RND Vx, NN - Set Vx = random byte AND NN).
 *
 * This test verifies that executing the CXNN opcode sets Vx to a random byte ANDed with NN.
 */
TEST_F(Chip8CPUTest, opcode_CXNN_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the DXYN opcode (DRW Vx, Vy, N - Display/draw sprite).
 *
 * This test verifies that executing the DXYN opcode draws a sprite at coordinate (Vx, Vy) with N
 * bytes of sprite data.
 */
TEST_F(Chip8CPUTest, opcode_DXYN_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the EX9E opcode (SKP Vx - Skip next instruction if key with the value of Vx is
 * pressed).
 *
 * This test verifies that executing the EX9E opcode skips the next instruction if the key
 * corresponding to Vx is pressed.
 */
TEST_F(Chip8CPUTest, opcode_EX9E_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the EXA1 opcode (SKNP Vx - Skip next instruction if key with the value of Vx is not
 * pressed).
 *
 * This test verifies that executing the EXA1 opcode skips the next instruction if the key
 * corresponding to Vx is not pressed.
 */
TEST_F(Chip8CPUTest, opcode_EXA1_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX07 opcode (LD Vx, DT - Set Vx = delay timer value).
 *
 * This test verifies that executing the FX07 opcode sets Vx to the value of the delay timer.
 */
TEST_F(Chip8CPUTest, opcode_FX07_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX0A opcode (LD Vx, K - Wait for a key press, store the value of the key in Vx).
 *
 * This test verifies that executing the FX0A opcode waits for a key press and stores the key value
 * in Vx.
 */
TEST_F(Chip8CPUTest, opcode_FX0A_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX15 opcode (LD DT, Vx - Set delay timer = Vx).
 *
 * This test verifies that executing the FX15 opcode sets the delay timer to the value of Vx.
 */
TEST_F(Chip8CPUTest, opcode_FX15_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX18 opcode (LD ST, Vx - Set sound timer = Vx).
 *
 * This test verifies that executing the FX18 opcode sets the sound timer to the value of Vx.
 */
TEST_F(Chip8CPUTest, opcode_FX18_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX1E opcode (ADD I, Vx - Set I = I + Vx).
 *
 * This test verifies that executing the FX1E opcode adds Vx to the index register I.
 */
TEST_F(Chip8CPUTest, opcode_FX1E_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX29 opcode (LD F, Vx - Set I = location of sprite for digit Vx).
 *
 * This test verifies that executing the FX29 opcode sets I to the location of the sprite for the
 * digit in Vx.
 */
TEST_F(Chip8CPUTest, opcode_FX29_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX33 opcode (LD B, Vx - Store BCD representation of Vx in memory locations I,
 * I+1, and I+2).
 *
 * This test verifies that executing the FX33 opcode stores the binary-coded decimal representation
 * of Vx in memory.
 */
TEST_F(Chip8CPUTest, opcode_FX33_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX55 opcode (LD [I], Vx - Store registers V0 through Vx in memory starting at
 * location I).
 *
 * This test verifies that executing the FX55 opcode stores registers V0 through Vx in memory
 * starting at address I.
 */
TEST_F(Chip8CPUTest, opcode_FX55_test)
{
    FAIL() << "Not yet implemented";
}

/**
 * @brief Test the FX65 opcode (LD Vx, [I] - Read registers V0 through Vx from memory starting at
 * location I).
 *
 * This test verifies that executing the FX65 opcode reads registers V0 through Vx from memory
 * starting at address I.
 */
TEST_F(Chip8CPUTest, opcode_FX65_test)
{
    FAIL() << "Not yet implemented";
}