#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <vector>

#include "Chip8Core/Chip8.h"
#include "Chip8Emulator/Chip8Display.h"
#include "Chip8Emulator/Chip8Input.h"
#include "Chip8Emulator/Chip8ROMLoader.h"

int main()
{
    spdlog::info("Application Started");
    chip8core::Chip8 chip8;
    Chip8ROMLoader::loadROM("../../../roms/1-chip8-logo.ch8", chip8);
    Chip8Display display(64, 32, 10);
    Chip8Input   input;
    bool         running = true;
    while (running)
    {
        input.pollEvents(chip8.getInput(), running);
        chip8.cycle();
        display.render(chip8.getGraphics());
        SDL_Delay(16); // ~60Hz
    }
    spdlog::info("Application Ended");
    return 0;
}