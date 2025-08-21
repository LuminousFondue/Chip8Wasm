#include "Chip8.h"

#include <spdlog/spdlog.h>

#include <cstdio>

Chip8::Chip8() : memory_(), graphics_(), cpu_(memory_, graphics_, input_, delayTimer_, soundTimer_)
{
    spdlog::debug("Chip8 Created");
}

Chip8::~Chip8()
{
    spdlog::debug("Chip8 Destroyed");
}

void Chip8::reset()
{
    cpu_.reset();
    graphics_.clear();
    memory_.clear();
    delayTimer_.reset();
    soundTimer_.reset();
    spdlog::debug("Chip8 reset to initial state");
}