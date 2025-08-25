#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

#include "Chip8Core/Chip8.h"

int main()
{
    spdlog::info("Application Started");
    chip8core::Chip8 chip8;
    spdlog::info("Application Ended");
    return 0;
}