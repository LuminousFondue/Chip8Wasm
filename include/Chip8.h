#pragma once

#include "Chip8CPU.h"
#include "Chip8GraphicsData.h"
#include "Chip8InputData.h"
#include "Chip8Memory.h"

class Chip8
{
  public:
    Chip8();
    ~Chip8();
    void reset();

  private:
    Chip8Memory       memory_;
    Chip8GraphicsData graphics_;
    Chip8InputData    input_;
    Chip8CPU          cpu_;
};