#include "Chip8Core/Chip8.h"

#include <spdlog/spdlog.h>

#include <cstdio>
namespace chip8core
{
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
    using namespace std::chrono;
    auto   now   = steady_clock::now();
    double delta = duration<double>(now - lastTick_).count();
    lastTick_    = now;

    cpuAccumulator_ += delta;
    timerAccumulator_ += delta;

    // Run as many CPU cycles as needed
    while (cpuAccumulator_ >= CPU_CYCLE_TIME)
    {
        cpu_.cycle();
        input_.syncKeyStates();
        cpuAccumulator_ -= CPU_CYCLE_TIME;
    }

    // Update timers at 60Hz
    while (timerAccumulator_ >= TIMER_CYCLE_TIME)
    {
        updateTimers();
        timerAccumulator_ -= TIMER_CYCLE_TIME;
    }
}

void Chip8::updateTimers()
{
    delayTimer_.update();
    soundTimer_.update();
}
} // namespace chip8core