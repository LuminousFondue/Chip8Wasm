#pragma once

#include <cstdio>

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
    Chip8CPU    _cpu;
    Chip8Memory _memory;
};