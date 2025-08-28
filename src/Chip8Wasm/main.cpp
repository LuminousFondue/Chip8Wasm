#include <SDL2/SDL.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <vector>

#include "Chip8Core/Chip8.h"
#include "Chip8Core/Chip8Timer.h"
#include "Chip8Emulator/Chip8Audio.h"
#include "Chip8Emulator/Chip8Display.h"
#include "Chip8Emulator/Chip8Input.h"
#include "Chip8Emulator/Chip8ROMLoader.h"

chip8core::Chip8 chip8;
Chip8Display     display(64, 32, 10);
Chip8Input       input;
Chip8Audio*      audio          = nullptr;
bool             running        = true;
bool             romLoaded      = false;
const int        cyclesPerFrame = 10;

void initAudio()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0)
    {
        audio = new Chip8Audio();
    }
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void load_rom(const char* filename)
    {
        romLoaded = Chip8ROMLoader::loadROM(filename, chip8);
        spdlog::info("ROM loaded: {}", filename);
        initAudio();
    }
}

bool emulationIteration(double time, void* userData)
{
    if (romLoaded)
    {
        // Poll for input
        input.pollEvents(chip8.getInput(), running);

        // Cycle Chip8
        chip8.cycle();

        // Render display
        display.render(chip8.getGraphics());

        // Play audio
        audio->processAudio(chip8.getSoundTimer());
    }

    return EM_TRUE;
}

int main()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Application Started");

    emscripten_request_animation_frame_loop(emulationIteration, 0);

    emscripten_exit_with_live_runtime();
    return 0;
}