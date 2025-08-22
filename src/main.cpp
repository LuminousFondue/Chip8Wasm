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
    std::vector<uint8_t> rom = loadRom("../../../roms/7-beep.ch8");
    chip8.loadROM(rom.data(), rom.size());
}

#include <atomic>
#include <chrono>
#include <thread>

std::atomic<bool> playToneFlag{false};

void audioCallback(void* userdata, Uint8* stream, int len)
{
    static double phase      = 0.0;
    double        freq       = 440.0; // A4 tone
    int           sampleRate = 44100;
    int16_t*      buffer     = reinterpret_cast<int16_t*>(stream);
    int           samples    = len / 2;

    if (playToneFlag)
    {
        for (int i = 0; i < samples; ++i)
        {
            buffer[i] = static_cast<int16_t>(std::sin(phase) * 3000);
            phase += 2.0 * M_PI * freq / sampleRate;
            if (phase > 2.0 * M_PI)
                phase -= 2.0 * M_PI;
        }
    }
    else
    {
        std::memset(stream, 0, len);
    }
}

void initAudio()
{
    SDL_AudioSpec want{}, have{};
    want.freq     = 44100;
    want.format   = AUDIO_S16SYS;
    want.channels = 1;
    want.samples  = 512;
    want.callback = audioCallback;

    if (SDL_OpenAudio(&want, &have) < 0)
    {
        spdlog::error("Failed to open audio: {}", SDL_GetError());
    }
    else
    {
        SDL_PauseAudio(0);
    }
}

void playTone(bool play)
{
    playToneFlag = play;
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

    initAudio();

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

        if (chip8.getSoundTimer().getValue() > 0)
        {
            spdlog::debug("Sound timer is active: {}", chip8.getSoundTimer().getValue());
            playTone(true);
        }
        else
        {
            playTone(false);
        }

        // Emulate one cycle of the Chip8 CPU
        chip8.cycle();

        // Draw Chip8 display
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        for (int y = 0; y < CHIP8_HEIGHT; ++y)
        {
            for (int x = 0; x < CHIP8_WIDTH; ++x)
            {
                if (chip8.getGraphics().getPixel(x, y))
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