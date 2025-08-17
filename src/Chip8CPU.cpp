#include "Chip8CPU.h"

#include <spdlog/spdlog.h>

#include <cstdio>

Chip8CPU::Chip8CPU(Chip8Memory& memory) : memory_(memory)
{
    reset();
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
}

void Chip8CPU::reset()
{
    PC_ = 0x200;                                // Reset program counter to start of program area
    SP_ = 0;                                    // Reset stack pointer
    I_  = 0;                                    // Reset index register
    std::fill(std::begin(V_), std::end(V_), 0); // Clear registers
    std::fill(std::begin(stack_), std::end(stack_), 0); // Clear stack
}
