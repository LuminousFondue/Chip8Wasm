#include "Chip8CPU.h"

#include <spdlog/spdlog.h>

#include <cstdio>

Chip8CPU::Chip8CPU(Chip8Memory& memory) : memory_(memory)
{
    spdlog::debug("Chip8 CPU created");
}

Chip8CPU::~Chip8CPU()
{
    spdlog::debug("Chip8 CPU destroyed");
}