#include "Chip8CPU.h"

#include <spdlog/spdlog.h>

#include <cstdio>

Chip8CPU::Chip8CPU(Chip8Memory& memory, Chip8GraphicsData& graphics)
    : memory_(memory), graphics_(graphics)
{
    reset();
    initializeOpcodeTables();
    spdlog::debug("Chip8 CPU created");
}

Chip8CPU::~Chip8CPU()
{
    spdlog::debug("Chip8 CPU destroyed");
}

void Chip8CPU::cycle()
{
    // Fetch opcode
    uint16_t opcode = memory_.read(PC_) << 8 | memory_.read(PC_ + 1);
    PC_ += 2;
    decodeOpcode(opcode);
}

void Chip8CPU::reset()
{
    PC_ = 0x200;                                // Reset program counter to start of program area
    SP_ = 0;                                    // Reset stack pointer
    I_  = 0;                                    // Reset index register
    std::fill(std::begin(V_), std::end(V_), 0); // Clear registers
    std::fill(std::begin(stack_), std::end(stack_), 0); // Clear stack
    spdlog::debug("Chip8 CPU reset to initial state");
}

uint8_t Chip8CPU::getNibble(uint16_t opcode, int nibbleIndex)
{
    return (opcode >> (nibbleIndex * 4)) & 0xF;
}

void Chip8CPU::initializeOpcodeTables()
{
    spdlog::debug("Initializing opcode tables");
    // Initialize all entries to invalidOpcode
    for (auto& handler : main_opcode_table_)
        handler = &Chip8CPU::invalidOpcode;
    for (auto& handler : _0_table)
        handler = &Chip8CPU::invalidOpcode;
    for (auto& handler : _8_table)
        handler = &Chip8CPU::invalidOpcode;
    for (auto& handler : _E_table)
        handler = &Chip8CPU::invalidOpcode;
    for (auto& handler : _F_table)
        handler = &Chip8CPU::invalidOpcode;

    // Initialize main opcode table
    main_opcode_table_[0x0] = &Chip8CPU::handle_0XXX;
    main_opcode_table_[0x1] = &Chip8CPU::opcode_1NNN;
    main_opcode_table_[0x2] = &Chip8CPU::opcode_2NNN;
    main_opcode_table_[0x3] = &Chip8CPU::opcode_3XKK;
    main_opcode_table_[0x4] = &Chip8CPU::opcode_4XKK;
    main_opcode_table_[0x5] = &Chip8CPU::opcode_5XY0;
    main_opcode_table_[0x6] = &Chip8CPU::opcode_6XKK;
    main_opcode_table_[0x7] = &Chip8CPU::opcode_7XKK;
    main_opcode_table_[0x8] = &Chip8CPU::handle_8XXX;
    main_opcode_table_[0x9] = &Chip8CPU::opcode_9XY0;
    main_opcode_table_[0xA] = &Chip8CPU::opcode_ANNN;
    main_opcode_table_[0xB] = &Chip8CPU::opcode_BNNN;
    main_opcode_table_[0xC] = &Chip8CPU::opcode_CXKK;
    main_opcode_table_[0xD] = &Chip8CPU::opcode_DXYN;
    main_opcode_table_[0xE] = &Chip8CPU::handle_EXXX;
    main_opcode_table_[0xF] = &Chip8CPU::handle_FXXX;

    // Initialize 0XXX opcode table
    _0_table[0x0] = &Chip8CPU::opcode_00E0;
    _0_table[0xE] = &Chip8CPU::opcode_00EE;

    // Initialize 8XXX opcode table
    _8_table[0x0] = &Chip8CPU::opcode_8XY0;
    _8_table[0x1] = &Chip8CPU::opcode_8XY1;
    _8_table[0x2] = &Chip8CPU::opcode_8XY2;
    _8_table[0x3] = &Chip8CPU::opcode_8XY3;
    _8_table[0x4] = &Chip8CPU::opcode_8XY4;
    _8_table[0x5] = &Chip8CPU::opcode_8XY5;
    _8_table[0x6] = &Chip8CPU::opcode_8XY6;
    _8_table[0x7] = &Chip8CPU::opcode_8XY7;
    _8_table[0xE] = &Chip8CPU::opcode_8XYE;

    // Initialize EXXX opcode table
    _E_table[0x1] = &Chip8CPU::opcode_EXA1;
    _E_table[0xE] = &Chip8CPU::opcode_EX9E;

    // Initialize FXXX opcode table
    _F_table[0x07] = &Chip8CPU::opcode_FX07;
    _F_table[0x0A] = &Chip8CPU::opcode_FX0A;
    _F_table[0x15] = &Chip8CPU::opcode_FX15;
    _F_table[0x18] = &Chip8CPU::opcode_FX18;
    _F_table[0x1E] = &Chip8CPU::opcode_FX1E;
    _F_table[0x29] = &Chip8CPU::opcode_FX29;
    _F_table[0x33] = &Chip8CPU::opcode_FX33;
    _F_table[0x55] = &Chip8CPU::opcode_FX55;
    _F_table[0x65] = &Chip8CPU::opcode_FX65;
}

void Chip8CPU::invalidOpcode(uint16_t opcode)
{
    spdlog::error("Invalid or unimplemented opcode: {:#04x}", opcode);
}

void Chip8CPU::decodeOpcode(uint16_t opcode)
{
    spdlog::debug("Decoding opcode: {:#04x}", opcode);

    // Extract the first nibble to determine the main opcode
    uint8_t main_opcode = (opcode >> 12) & 0xF;

    // Call the appropriate handler based on the main opcode
    if (main_opcode < main_opcode_table_.size())
    {
        (this->*main_opcode_table_[main_opcode])(opcode);
    }
    else
    {
        spdlog::error("Unknown opcode: {:#04x}", opcode);
    }
}

void Chip8CPU::handle_0XXX(uint16_t opcode)
{
    spdlog::debug("Handling 0XXX opcode: {:#04x}", opcode);

    // Extract the fourth nibble to determine the specific 0XXX opcode
    uint8_t sub_opcode = opcode & 0x000F;

    if (sub_opcode < _0_table.size())
    {
        (this->*_0_table[sub_opcode])(opcode);
    }
    else
    {
        spdlog::error("Unknown 0XXX opcode: {:#04x}", opcode);
    }
}

void Chip8CPU::handle_8XXX(uint16_t opcode)
{
    spdlog::debug("Handling 8XXX opcode: {:#04x}", opcode);

    uint8_t sub_opcode = opcode & 0x000F;

    if (sub_opcode < _8_table.size())
    {
        (this->*_8_table[sub_opcode])(opcode);
    }
    else
    {
        spdlog::error("Unknown 8XXX opcode: {:#04x}", opcode);
    }
}

void Chip8CPU::handle_EXXX(uint16_t opcode)
{
    spdlog::debug("Handling EXXX opcode: {:#04x}", opcode);

    // Extract the fourth nibble to determine the specific EXXX opcode
    uint8_t sub_opcode = opcode & 0x000F;

    if (sub_opcode < _E_table.size())
    {
        (this->*_E_table[sub_opcode])(opcode);
    }
    else
    {
        spdlog::error("Unknown EXXX opcode: {:#04x}", opcode);
    }
}

void Chip8CPU::handle_FXXX(uint16_t opcode)
{
    spdlog::debug("Handling FXXX opcode: {:#04x}", opcode);

    // Extract the fourth nibble to determine the specific FXXX opcode
    uint8_t sub_opcode = opcode & 0x00FF;

    if (sub_opcode < _F_table.size())
    {
        (this->*_F_table[sub_opcode])(opcode);
    }
    else
    {
        spdlog::error("Unknown FXXX opcode: {:#04x}", opcode);
    }
}

/**
 * CLS - Clear the Display
 */
void Chip8CPU::opcode_00E0(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 00E0");
    graphics_.clear();
}

/**
 * 00EE: RET - Return from a subroutine
 *
 * The interpreter sets the program counter to the
 * address at the top of the stack,
 * then subtracts 1 from the stack pointer.
 */
void Chip8CPU::opcode_00EE(uint16_t opcode)
{
    /* TODO: implement 00EE (RET) */
    spdlog::debug("Opcode: 00EE");
}

/**
 * 1NNN: JP addr - Jump to location nnn
 *
 * The interpreter sets the program counter to nnn.
 */
void Chip8CPU::opcode_1NNN(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 1NNN");
    uint16_t address = opcode & 0x0FFF;
    this->PC_        = address;
}

/**
 * 2NNN: CALL addr - Call subroutine at nnn
 *
 * The interpreter increments the stack pointer,
 * then puts the current PC on the top of the stack.
 * The PC is then set to nnn.
 */
void Chip8CPU::opcode_2NNN(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 2NNN");
    uint16_t address = opcode & 0x0FFF;
    this->SP_++;
    this->stack_[this->SP_] = this->PC_;
    this->PC_               = address;
}

/**
 * 3XKK: SE Vx, byte - Skip next instruction if Vx == kk
 *
 * The interpreter compares register Vx to kk, and if they
 * are equal, increments the program counter by 2.
 */
void Chip8CPU::opcode_3XKK(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 3XKK");
    uint8_t kk = opcode & 0xFF;
    uint8_t x  = this->getNibble(opcode, 2);
    if (this->getV(x) == kk)
    {
        this->PC_ += 2;
    }
}

/**
 * 4XKK: SNE Vx, byte - Skip next instruction if Vx != kk
 *
 * The interpreter compares register Vx to kk, and if they
 * are not equal, increments the program counter by 2.
 */
void Chip8CPU::opcode_4XKK(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 4XKK");
    uint8_t kk = opcode & 0xFF;
    uint8_t x  = this->getNibble(opcode, 2);
    if (this->getV(x) != kk)
    {
        this->PC_ += 2;
    }
}

/**
 * 5XY0: SE Vx, Vy - Skip next instruction if Vx == Vy
 *
 * The interpreter compares register Vx to register Vy,
 * and if they are equal, increments the program counter by 2.
 */
void Chip8CPU::opcode_5XY0(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 5XY0");
    uint8_t x = this->getNibble(opcode, 2);
    uint8_t y = this->getNibble(opcode, 1);
    if (this->getV(x) == this->getV(y))
    {
        this->PC_ += 2;
    }
}

/**
 * 6XKK: LD Vx, byte - Set Vx = kk
 *
 * The interpreter loads the value kk into register Vx.
 */
void Chip8CPU::opcode_6XKK(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 6XKK");
    uint8_t x  = this->getNibble(opcode, 2);
    uint8_t kk = opcode & 0xFF;
    this->setV(x, kk);
}

/**
 * 7XKK: ADD Vx, byte - Set Vx = Vx + kk
 *
 * Adds the value kk to the value of register Vx,
 * then stores the result in Vx.
 */
void Chip8CPU::opcode_7XKK(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 7XKK");
    uint8_t x  = this->getNibble(opcode, 2);
    uint8_t kk = opcode & 0xFF;
    this->setV(x, this->getV(x) + kk);
}

/**
 * 8XY0: LD Vx, Vy - Set Vx = Vy
 *
 * Stores the value of register Vy in register Vx.
 */
void Chip8CPU::opcode_8XY0(uint16_t opcode)
{
    spdlog::debug("Running Opcode: 8XY0");
    uint8_t x = this->getNibble(opcode, 2);
    uint8_t y = this->getNibble(opcode, 1);
    this->setV(x, this->getV(y));
}

/**
 * 8XY1: OR Vx, Vy - Set Vx = Vx OR Vy
 *
 * Performs a bitwise OR on the values of Vx and Vy,
 * then stores the result in Vx. A bitwise OR compares the
 * corresponding bits from two values, and if either bit is 1,
 * then the same bit in the result is also 1. Otherwise, it is 0.
 */
void Chip8CPU::opcode_8XY1(uint16_t opcode)
{
    /* TODO: implement 8XY1 (OR Vx, Vy) */
    spdlog::debug("Opcode: 8XY1");
}

/**
 * 8XY2: AND Vx, Vy - Set Vx = Vx AND Vy
 *
 * Performs a bitwise AND on the values of Vx and Vy,
 * then stores the result in Vx. A bitwise AND compares the
 * corresponding bits from two values, and if both bits are 1,
 * then the same bit in the result is also 1. Otherwise, it is 0.
 */
void Chip8CPU::opcode_8XY2(uint16_t opcode)
{
    /* TODO: implement 8XY2 (AND Vx, Vy) */
    spdlog::debug("Opcode: 8XY2");
}

/**
 * 8XY3: XOR Vx, Vy - Set Vx = Vx XOR Vy
 *
 * Performs a bitwise XOR on the values of Vx and Vy,
 * then stores the result in Vx. A bitwise XOR compares the
 * corresponding bits from two values, and if the bits are different,
 * then the same bit in the result is 1. Otherwise, it is 0.
 */
void Chip8CPU::opcode_8XY3(uint16_t opcode)
{
    /* TODO: implement 8XY3 (XOR Vx, Vy) */
    spdlog::debug("Opcode: 8XY3");
}

/**
 * 8XY4: ADD Vx, Vy - Set Vx = Vx + Vy
 *
 * The values of Vx and Vy are added together.
 * If the result is greater than 8 bits (i.e., > 255,)
 * VF is set to 1, otherwise 0. Only the lowest 8 bits
 * of the result are kept, and stored in Vx.
 */
void Chip8CPU::opcode_8XY4(uint16_t opcode)
{
    /* TODO: implement 8XY4 (ADD Vx, Vy) */
    spdlog::debug("Opcode: 8XY4");
}

/**
 * 8XY5: SUB Vx, Vy - Set Vx = Vx - Vy
 *
 * If Vx > Vy, then VF is set to 1, otherwise 0.
 * Then Vy is subtracted from Vx, and the results stored in Vx.
 */
void Chip8CPU::opcode_8XY5(uint16_t opcode)
{
    /* TODO: implement 8XY5 (SUB Vx, Vy) */
    spdlog::debug("Opcode: 8XY5");
}

/**
 * 8XY6: SHR Vx {, Vy} - Set Vx = Vx SHR 1
 *
 * If the least-significant bit of Vx is 1,
 * then VF is set to 1, otherwise 0. Then Vx is divided by 2.
 */
void Chip8CPU::opcode_8XY6(uint16_t opcode)
{
    /* TODO: implement 8XY6 (SHR Vx {, Vy}) */
    spdlog::debug("Opcode: 8XY6");
}

/**
 * 8XY7: SUBN Vx, Vy - Set Vx = Vy - Vx
 *
 * If Vy > Vx, then VF is set to 1, otherwise 0.
 * Then Vx is subtracted from Vy, and the results stored in Vx.
 */
void Chip8CPU::opcode_8XY7(uint16_t opcode)
{
    /* TODO: implement 8XY7 (SUBN Vx, Vy) */
    spdlog::debug("Opcode: 8XY7");
}

/**
 * 8XYE: SHL Vx {, Vy} - Set Vx = Vx SHL 1
 *
 * If the most-significant bit of Vx is 1,
 * then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
 */
void Chip8CPU::opcode_8XYE(uint16_t opcode)
{
    /* TODO: implement 8XYE (SHL Vx {, Vy}) */
    spdlog::debug("Opcode: 8XYE");
}

/**
 * 9XY0: SNE Vx, Vy - Skip next instruction if Vx != Vy
 *
 * The values of Vx and Vy are compared, and if they are not equal,
 * the program counter is increased by 2.
 */
void Chip8CPU::opcode_9XY0(uint16_t opcode)
{
    /* TODO: implement 9XY0 (SNE Vx, Vy) */
    spdlog::debug("Opcode: 9XY0");
}

/**
 * ANNN: LD I, addr - Set I = addr
 *
 * The value of register I is set to nnn.
 */
void Chip8CPU::opcode_ANNN(uint16_t opcode)
{
    /* TODO: implement ANNN (LD I, addr) */
    spdlog::debug("Opcode: ANNN");
}

/**
 * BNNN: JP V0, addr - Jump to location nnn + V0
 *
 * The program counter is set to nnn plus the value of V0.
 */
void Chip8CPU::opcode_BNNN(uint16_t opcode)
{
    /* TODO: implement BNNN (JP V0, addr) */
    spdlog::debug("Opcode: BNNN");
}

/**
 * CXKK: RND Vx, byte - Set Vx = random byte AND KK
 *
 * The interpreter generates a random number from 0 to 255,
 * which is then ANDed with the value kk. The results are stored in Vx.
 * See instruction 8xy2 for more information on AND.
 */
void Chip8CPU::opcode_CXKK(uint16_t opcode)
{
    /* TODO: implement CXKK (RND Vx, byte) */
    spdlog::debug("Opcode: CXKK");
}

/**
 * DXYN: DRW Vx, Vy, nibble - Draw sprite at (Vx, Vy)
 *
 * The interpreter reads n bytes from memory, starting at
 * the address stored in I. These bytes are then displayed as
 * sprites on screen at coordinates (Vx, Vy). Sprites are XORed
 * onto the existing screen. If this causes any pixels to be erased,
 * VF is set to 1, otherwise it is set to 0. If the sprite is positioned
 * so part of it is outside the coordinates of the display, it wraps around
 * to the opposite side of the screen. See instruction 8xy3 for more information
 * on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
 */
void Chip8CPU::opcode_DXYN(uint16_t opcode)
{
    /* TODO: implement DXYN (DRW Vx, Vy, nibble) */
    spdlog::debug("Opcode: DXYN");
}

/**
 * EX9E: SKP Vx - Skip next instruction if key with value Vx is pressed
 *
 * Checks the keyboard, and if the key corresponding to the value of
 * Vx is currently in the down position, PC is increased by 2.
 */
void Chip8CPU::opcode_EX9E(uint16_t opcode)
{
    /* TODO: implement EX9E (SKP Vx) */
    spdlog::debug("Opcode: EX9E");
}

/**
 * EXA1: SKNP Vx - Skip next instruction if key with value Vx is not pressed
 *
 * Checks the keyboard, and if the key corresponding to the value of
 * Vx is currently not in the down position, PC is increased by 2.
 */
void Chip8CPU::opcode_EXA1(uint16_t opcode)
{
    /* TODO: implement EXA1 (SKNP Vx) */
    spdlog::debug("Opcode: EXA1");
}

/**
 * FX07: LD Vx, DT - Set Vx = delay timer value
 *
 * The value of the delay timer is copied into register Vx.
 */
void Chip8CPU::opcode_FX07(uint16_t opcode)
{
    /* TODO: implement FX07 (LD Vx, DT) */
    spdlog::debug("Opcode: FX07");
}

/**
 * FX0A: LD Vx, K - Wait for a key press, store the value of the key in Vx
 *
 * The interpreter waits for a key press, and when a key is pressed,
 * the value of the key is stored in register Vx.
 */
void Chip8CPU::opcode_FX0A(uint16_t opcode)
{
    /* TODO: implement FX0A (LD Vx, K) */
    spdlog::debug("Opcode: FX0A");
}

/**
 * FX15: LD DT, Vx - Set delay timer = Vx
 *
 * The value of register Vx is copied into the delay timer.
 */
void Chip8CPU::opcode_FX15(uint16_t opcode)
{
    /* TODO: implement FX15 (LD DT, Vx) */
    spdlog::debug("Opcode: FX15");
}

/**
 * FX18: LD ST, Vx - Set sound timer = Vx
 *
 * The value of register Vx is copied into the sound timer.
 */
void Chip8CPU::opcode_FX18(uint16_t opcode)
{
    /* TODO: implement FX18 (LD ST, Vx) */
    spdlog::debug("Opcode: FX18");
}

/**
 * FX1E: ADD I, Vx - Add Vx to I
 *
 * The value of register Vx is added to the value of register I.
 */
void Chip8CPU::opcode_FX1E(uint16_t opcode)
{
    /* TODO: implement FX1E (ADD I, Vx) */
    spdlog::debug("Opcode: FX1E");
}

/**
 * FX29: LD F, Vx - Load font sprite for digit Vx into I
 *
 * The value of register Vx is used as an index into the font sprite
 * data stored in memory. The address of the sprite is loaded into
 * register I.
 */
void Chip8CPU::opcode_FX29(uint16_t opcode)
{
    /* TODO: implement FX29 (LD F, Vx) */
    spdlog::debug("Opcode: FX29");
}

/**
 * FX33: LD B, Vx - Store BCD representation of Vx in memory
 *
 * The interpreter takes the value of register Vx, converts it to Binary-Coded Decimal (BCD),
 * and stores the hundreds digit at address I, the tens digit at I+1, and the ones digit at I+2.
 */
void Chip8CPU::opcode_FX33(uint16_t opcode)
{
    /* TODO: implement FX33 (LD B, Vx) */
    spdlog::debug("Opcode: FX33");
}

/**
 * FX55: LD [I], Vx - Store registers V0 through Vx in memory
 *
 * The interpreter copies the values of registers V0 through Vx into
 * memory, starting at the address stored in I.
 */
void Chip8CPU::opcode_FX55(uint16_t opcode)
{
    /* TODO: implement FX55 (LD [I], Vx) */
    spdlog::debug("Opcode: FX55");
}

/**
 * FX65: LD Vx, [I] - Read registers V0 through Vx from memory
 *
 * The interpreter copies the values from memory, starting at the address
 * stored in I, into registers V0 through Vx.
 */
void Chip8CPU::opcode_FX65(uint16_t opcode)
{
    spdlog::debug("Running Opcode: FX65");
    uint8_t x = this->getNibble(opcode, 2);
    for (uint8_t i = 0; i <= x; ++i)
    {
        this->setV(i, memory_.read(this->getI() + i));
    }
}