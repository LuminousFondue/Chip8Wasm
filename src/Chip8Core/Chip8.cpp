#include "Chip8Core/Chip8.h"

#include <spdlog/spdlog.h>

#include <cstdio>
namespace chip8core
{
Chip8::Chip8()
    : memory_(), graphics_(), cpu_(memory_, graphics_, input_, delayTimer_, soundTimer_),
      isPaused_(false)
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

void Chip8::loadROM(const uint8_t* romData, size_t romSize)
{
    for (size_t i = 0; i < romSize && i < memory_.MEMORY_SIZE; ++i)
    {
        memory_.write(0x200 + i, romData[i]);
    }
    spdlog::info("ROM loaded into memory");
}

void Chip8::cycle()
{
    if (!isPaused_)
    {
        cpu_.cycle();
        delayTimer_.update();
        soundTimer_.update();
    }
}

void Chip8::pause()
{
    isPaused_ = true;
}

void Chip8::resume()
{
    isPaused_ = false;
}
} // namespace chip8core