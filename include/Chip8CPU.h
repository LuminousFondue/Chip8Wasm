#pragma once

#include "Chip8Memory.h"

class Chip8CPU
{
  public:
    explicit Chip8CPU(Chip8Memory& memory);
    ~Chip8CPU();

  private:
    Chip8Memory& memory_;
};