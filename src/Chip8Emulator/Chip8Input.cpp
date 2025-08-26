#include "Chip8Emulator/Chip8Input.h"

#include <spdlog/spdlog.h>

void Chip8Input::pollEvents(chip8core::Chip8InputBuffer& input, bool& running)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
            return;
        }
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        {
            running = false;
            return;
        }
    }

    // Poll the current state of all keys
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Map SDL scancodes to Chip8 keys
    input.setKeyState(0x1, state[SDL_SCANCODE_1]);
    input.setKeyState(0x2, state[SDL_SCANCODE_2]);
    input.setKeyState(0x3, state[SDL_SCANCODE_3]);
    input.setKeyState(0xC, state[SDL_SCANCODE_4]);
    input.setKeyState(0x4, state[SDL_SCANCODE_Q]);
    input.setKeyState(0x5, state[SDL_SCANCODE_W]);
    input.setKeyState(0x6, state[SDL_SCANCODE_E]);
    input.setKeyState(0xD, state[SDL_SCANCODE_R]);
    input.setKeyState(0x7, state[SDL_SCANCODE_A]);
    input.setKeyState(0x8, state[SDL_SCANCODE_S]);
    input.setKeyState(0x9, state[SDL_SCANCODE_D]);
    input.setKeyState(0xE, state[SDL_SCANCODE_F]);
    input.setKeyState(0xA, state[SDL_SCANCODE_Z]);
    input.setKeyState(0x0, state[SDL_SCANCODE_X]);
    input.setKeyState(0xB, state[SDL_SCANCODE_C]);
    input.setKeyState(0xF, state[SDL_SCANCODE_V]);
}