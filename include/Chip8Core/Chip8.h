#pragma once

#include <chrono>

#include "Chip8Core/Chip8CPU.h"
#include "Chip8Core/Chip8GraphicsBuffer.h"
#include "Chip8Core/Chip8InputBuffer.h"
#include "Chip8Core/Chip8Memory.h"
#include "Chip8Core/Chip8Timer.h"

namespace chip8core
{
class Chip8
{
    static constexpr double CPU_CYCLE_TIME   = 1.0 / 700.0; // 700Hz
    static constexpr double TIMER_CYCLE_TIME = 1.0 / 60.0;  // 60Hz

  public:
    Chip8();
    ~Chip8();
    void reset();
    void loadROM(const uint8_t* romData, size_t romSize);
    void cycle();

    const chip8core::Chip8GraphicsBuffer& getGraphics() const { return graphics_; }
    const chip8core::Chip8Timer&          getSoundTimer() const { return soundTimer_; }
    chip8core::Chip8InputBuffer&          getInput() { return input_; }

  private:
    chip8core::Chip8Memory         memory_;
    chip8core::Chip8GraphicsBuffer graphics_;
    chip8core::Chip8InputBuffer    input_;
    chip8core::Chip8Timer          delayTimer_;
    chip8core::Chip8Timer          soundTimer_;
    chip8core::Chip8CPU            cpu_;
    double                         cpuAccumulator_   = 0.0;
    double                         timerAccumulator_ = 0.0;

    std::chrono::steady_clock::time_point lastTick_ = std::chrono::steady_clock::now();
    void                                  updateTimers();
};
} // namespace chip8core