#pragma once

#include <string>

#include "Chip8Core/Chip8.h"

class Chip8ROMLoader
{
  public:
    Chip8ROMLoader() = delete; // Prevent instantiation
    static bool loadROM(const std::string& filename, chip8core::Chip8& chip8);
};