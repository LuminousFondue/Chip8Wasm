#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "Chip8.h"

const int WINDOW_SCALE = 10;
const int CHIP8_WIDTH  = 64;
const int CHIP8_HEIGHT = 32;

Chip8 chip8;

std::vector<uint8_t> loadRom(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        spdlog::error("Failed to open ROM file: {}", filename);
        return {};
    }
    spdlog::info("Successfully loaded ROM file: {}", filename);
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});
}

void loadROM()
{
    std::vector<uint8_t> rom = loadRom("../../../roms/4-flags.ch8");
    chip8.loadROM(rom.data(), rom.size());
}

int main()
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Application Started");
    loadROM();

    // SDL2 initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        spdlog::error("SDL could not initialize! SDL_Error: {}", SDL_GetError());
        return 1;
    }

    SDL_Window* window =
        SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         CHIP8_WIDTH * WINDOW_SCALE, CHIP8_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool      quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        // Emulate one cycle of the Chip8 CPU
        chip8.cycle();

        // Draw Chip8 display
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Example: draw pixels (replace with your Chip8 graphics buffer)
        for (int y = 0; y < CHIP8_HEIGHT; ++y)
        {
            for (int x = 0; x < CHIP8_WIDTH; ++x)
            {
                // Replace with your actual pixel check:
                if (chip8.getGraphics().getPixel(x,
                                                 y)) // Implement getPixel() or access your buffer
                {
                    SDL_Rect rect = {x * WINDOW_SCALE, y * WINDOW_SCALE, WINDOW_SCALE,
                                     WINDOW_SCALE};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White pixel
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        SDL_RenderPresent(renderer);

        // Emulate at ~60Hz
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    spdlog::info("Application Ended");
    return 0;
}