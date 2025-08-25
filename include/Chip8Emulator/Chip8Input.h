#pragma once
#include <SDL2/SDL.h>

#include "Chip8Core/Chip8InputBuffer.h"

class Chip8Input
{
  public:
    void pollEvents(chip8core::Chip8InputBuffer& input, bool& running);
};