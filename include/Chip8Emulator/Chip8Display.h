#pragma once
#include <SDL2/SDL.h>

#include "Chip8Core/Chip8GraphicsBuffer.h"

class Chip8Display
{
  public:
    Chip8Display(int width, int height, int scale);
    ~Chip8Display();
    void render(const chip8core::Chip8GraphicsBuffer& buffer);

  private:
    SDL_Window*   window_;
    SDL_Renderer* renderer_;
    int           width_, height_, scale_;
};