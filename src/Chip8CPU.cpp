#include "Chip8CPU.h"

#include <spdlog/spdlog.h>

#include <cstdio>

Chip8CPU::Chip8CPU(Chip8Memory& memory) : memory_(memory)
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

void Chip8CPU::initializeOpcodeTables()
{
    spdlog::debug("Initializing opcode tables");

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

    // Extract the fourth nibble to determine the specific 8XXX opcode
    uint8_t sub_opcode = (opcode >> 4) & 0x000F;

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
    uint8_t sub_opcode = (opcode >> 4) & 0x000F;

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
    uint8_t sub_opcode = (opcode >> 4) & 0x000F;

    if (sub_opcode < _F_table.size())
    {
        (this->*_F_table[sub_opcode])(opcode);
    }
    else
    {
        spdlog::error("Unknown FXXX opcode: {:#04x}", opcode);
    }
}

void Chip8CPU::opcode_00E0(uint16_t opcode)
{
    /* TODO: implement 00E0 (CLS) */
    spdlog::debug("Opcode: 00E0");
}

void Chip8CPU::opcode_00EE(uint16_t opcode)
{
    /* TODO: implement 00EE (RET) */
    spdlog::debug("Opcode: 00EE");
}

void Chip8CPU::opcode_1NNN(uint16_t opcode)
{
    /* TODO: implement 1NNN (JP addr) */
    spdlog::debug("Opcode: 1NNN");
}

void Chip8CPU::opcode_2NNN(uint16_t opcode)
{
    /* TODO: implement 2NNN (CALL addr) */
    spdlog::debug("Opcode: 2NNN");
}

void Chip8CPU::opcode_3XKK(uint16_t opcode)
{
    /* TODO: implement 3XKK (SE Vx, byte) */
    spdlog::debug("Opcode: 3XKK");
}

void Chip8CPU::opcode_4XKK(uint16_t opcode)
{
    /* TODO: implement 4XKK (SNE Vx, byte) */
    spdlog::debug("Opcode: 4XKK");
}

void Chip8CPU::opcode_5XY0(uint16_t opcode)
{
    /* TODO: implement 5XY0 (SE Vx, Vy) */
    spdlog::debug("Opcode: 5XY0");
}

void Chip8CPU::opcode_6XKK(uint16_t opcode)
{
    /* TODO: implement 6XKK (LD Vx, byte) */
    spdlog::debug("Opcode: 6XKK");
}

void Chip8CPU::opcode_7XKK(uint16_t opcode)
{
    /* TODO: implement 7XKK (ADD Vx, byte) */
    spdlog::debug("Opcode: 7XKK");
}

void Chip8CPU::opcode_8XY0(uint16_t opcode)
{
    /* TODO: implement 8XY0 (LD Vx, Vy) */
    spdlog::debug("Opcode: 8XY0");
}

void Chip8CPU::opcode_8XY1(uint16_t opcode)
{
    /* TODO: implement 8XY1 (OR Vx, Vy) */
    spdlog::debug("Opcode: 8XY1");
}

void Chip8CPU::opcode_8XY2(uint16_t opcode)
{
    /* TODO: implement 8XY2 (AND Vx, Vy) */
    spdlog::debug("Opcode: 8XY2");
}

void Chip8CPU::opcode_8XY3(uint16_t opcode)
{
    /* TODO: implement 8XY3 (XOR Vx, Vy) */
    spdlog::debug("Opcode: 8XY3");
}

void Chip8CPU::opcode_8XY4(uint16_t opcode)
{
    /* TODO: implement 8XY4 (ADD Vx, Vy) */
    spdlog::debug("Opcode: 8XY4");
}

void Chip8CPU::opcode_8XY5(uint16_t opcode)
{
    /* TODO: implement 8XY5 (SUB Vx, Vy) */
    spdlog::debug("Opcode: 8XY5");
}

void Chip8CPU::opcode_8XY6(uint16_t opcode)
{
    /* TODO: implement 8XY6 (SHR Vx {, Vy}) */
    spdlog::debug("Opcode: 8XY6");
}

void Chip8CPU::opcode_8XY7(uint16_t opcode)
{
    /* TODO: implement 8XY7 (SUBN Vx, Vy) */
    spdlog::debug("Opcode: 8XY7");
}

void Chip8CPU::opcode_8XYE(uint16_t opcode)
{
    /* TODO: implement 8XYE (SHL Vx {, Vy}) */
    spdlog::debug("Opcode: 8XYE");
}

void Chip8CPU::opcode_9XY0(uint16_t opcode)
{
    /* TODO: implement 9XY0 (SNE Vx, Vy) */
    spdlog::debug("Opcode: 9XY0");
}

void Chip8CPU::opcode_ANNN(uint16_t opcode)
{
    /* TODO: implement ANNN (LD I, addr) */
    spdlog::debug("Opcode: ANNN");
}

void Chip8CPU::opcode_BNNN(uint16_t opcode)
{
    /* TODO: implement BNNN (JP V0, addr) */
    spdlog::debug("Opcode: BNNN");
}

void Chip8CPU::opcode_CXKK(uint16_t opcode)
{
    /* TODO: implement CXKK (RND Vx, byte) */
    spdlog::debug("Opcode: CXKK");
}

void Chip8CPU::opcode_DXYN(uint16_t opcode)
{
    /* TODO: implement DXYN (DRW Vx, Vy, nibble) */
    spdlog::debug("Opcode: DXYN");
}

void Chip8CPU::opcode_EXA1(uint16_t opcode)
{
    /* TODO: implement EXA1 (SKNP Vx) */
    spdlog::debug("Opcode: EXA1");
}

void Chip8CPU::opcode_EX9E(uint16_t opcode)
{
    /* TODO: implement EX9E (SKP Vx) */
    spdlog::debug("Opcode: EX9E");
}

void Chip8CPU::opcode_FX07(uint16_t opcode)
{
    /* TODO: implement FX07 (LD Vx, DT) */
    spdlog::debug("Opcode: FX07");
}

void Chip8CPU::opcode_FX0A(uint16_t opcode)
{
    /* TODO: implement FX0A (LD Vx, K) */
    spdlog::debug("Opcode: FX0A");
}

void Chip8CPU::opcode_FX15(uint16_t opcode)
{
    /* TODO: implement FX15 (LD DT, Vx) */
    spdlog::debug("Opcode: FX15");
}

void Chip8CPU::opcode_FX18(uint16_t opcode)
{
    /* TODO: implement FX18 (LD ST, Vx) */
    spdlog::debug("Opcode: FX18");
}

void Chip8CPU::opcode_FX1E(uint16_t opcode)
{
    /* TODO: implement FX1E (ADD I, Vx) */
    spdlog::debug("Opcode: FX1E");
}

void Chip8CPU::opcode_FX29(uint16_t opcode)
{
    /* TODO: implement FX29 (LD F, Vx) */
    spdlog::debug("Opcode: FX29");
}

void Chip8CPU::opcode_FX33(uint16_t opcode)
{
    /* TODO: implement FX33 (LD B, Vx) */
    spdlog::debug("Opcode: FX33");
}

void Chip8CPU::opcode_FX55(uint16_t opcode)
{
    /* TODO: implement FX55 (LD [I], Vx) */
    spdlog::debug("Opcode: FX55");
}

void Chip8CPU::opcode_FX65(uint16_t opcode)
{
    /* TODO: implement FX65 (LD Vx, [I]) */
    spdlog::debug("Opcode: FX65");
}