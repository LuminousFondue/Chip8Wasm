#pragma once

#include <SDL2/SDL.h>

#include <atomic>

#include "Chip8Core/Chip8Timer.h"

class Chip8Audio
{
  public:
    Chip8Audio();
    ~Chip8Audio();
    void processAudio(const chip8core::Chip8Timer& soundTimer);

  private:
    SDL_AudioDeviceID device_;
    SDL_AudioSpec     spec_;
    std::atomic<bool> playing_;
    int               phase_;
    void              play();
    void              stop();
    static void       audioCallback(void* userdata, Uint8* stream, int len);
};