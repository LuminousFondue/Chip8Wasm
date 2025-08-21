#include "Chip8.h"

#include <spdlog/spdlog.h>

#include <cstdio>

Chip8::Chip8() : memory_(), graphics_(), cpu_(memory_, graphics_, input_, delayTimer_, soundTimer_)
{
    spdlog::debug("Chip8 Created");
    cpu_.setV(1, 0xA);
    cpu_.setV(2, 0x1);
    cpu_.setV(3, 0x1);

    memory_.write(0x200, 0xF1);
    memory_.write(0x201, 0x29);
    memory_.write(0x202, 0xD2);
    memory_.write(0x203, 0x35);

    cpu_.cycle();
    cpu_.cycle();

    graphics_.printScreen();
}

Chip8::~Chip8()
{
    spdlog::debug("Chip8 Destroyed");
}