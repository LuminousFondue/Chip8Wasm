#include "Chip8Emulator/Chip8Display.h"

Chip8Display::Chip8Display(int width, int height, int scale)
    : width_(width), height_(height), scale_(scale)
{
    SDL_Init(SDL_INIT_VIDEO);
    window_   = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width_ * scale_, height_ * scale_, SDL_WINDOW_SHOWN);
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
}

Chip8Display::~Chip8Display()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Chip8Display::render(const chip8core::Chip8GraphicsBuffer& buffer)
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            if (buffer.getPixel(x, y))
            {
                SDL_Rect rect = {x * scale_, y * scale_, scale_, scale_};
                SDL_RenderFillRect(renderer_, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer_);
}