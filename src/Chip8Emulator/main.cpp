#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <vector>

#include "Chip8Core/Chip8.h"
#include "Chip8Core/Chip8Timer.h"
#include "Chip8Emulator/Chip8Audio.h"
#include "Chip8Emulator/Chip8Display.h"
#include "Chip8Emulator/Chip8Input.h"
#include "Chip8Emulator/Chip8ROMLoader.h"

int main()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return -1;
    }
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Application Started");
    chip8core::Chip8             chip8;
    const chip8core::Chip8Timer& soundTimer = chip8.getSoundTimer();
    Chip8ROMLoader::loadROM("../../../roms/6-keypad.ch8", chip8);
    Chip8Display display(64, 32, 10);
    Chip8Input   input;
    Chip8Audio   audio;
    bool         running        = true;
    const int    cyclesPerFrame = 10;

    while (running)
    {
        input.pollEvents(chip8.getInput(), running);
        chip8.cycle();
        display.render(chip8.getGraphics());
        audio.processAudio(soundTimer);
        SDL_Delay(1);
    }
    spdlog::info("Application Ended");
    return 0;
}