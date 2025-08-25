#include "Chip8Emulator/Chip8Input.h"

void Chip8Input::pollEvents(chip8core::Chip8InputBuffer& input, bool& running)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            running = false;
        // Map SDL key events to input buffer here
    }
}