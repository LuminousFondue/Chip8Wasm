#pragma once

#include "Chip8CPU.h"
#include "Chip8GraphicsData.h"
#include "Chip8InputData.h"
#include "Chip8Memory.h"
#include "Chip8Timer.h"

class Chip8
{
  public:
    Chip8();
    ~Chip8();
    void reset();
    void loadROM(const uint8_t* romData, size_t romSize);
    void run();
    void pause();
    void stop();

  private:
    Chip8Memory       memory_;
    Chip8GraphicsData graphics_;
    Chip8InputData    input_;
    Chip8Timer        delayTimer_;
    Chip8Timer        soundTimer_;
    Chip8CPU          cpu_;
};