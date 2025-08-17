#pragma once

#include "Chip8CPU.h"
#include "Chip8Memory.h"

class Chip8
{
  public:
    Chip8();
    ~Chip8();
    void reset();
    void loadROM();

  private:
    Chip8Memory memory_;
    Chip8CPU    cpu_;
};