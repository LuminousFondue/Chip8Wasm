#include "Chip8Emulator/Chip8Audio.h"

#include <spdlog/spdlog.h>

Chip8Audio::Chip8Audio() : device_(0), playing_(false), phase_(0)
{
    spdlog::debug("Initializing Chip8Audio");
    SDL_zero(spec_);
    spec_.freq     = 44100;
    spec_.format   = AUDIO_U8;
    spec_.channels = 1;
    spec_.samples  = 512;
    spec_.callback = Chip8Audio::audioCallback;
    spec_.userdata = this;

    device_ = SDL_OpenAudioDevice(nullptr, 0, &spec_, nullptr, 0);
    spdlog::debug("Device: {}", device_);
    if (device_ == 0)
    {
        spdlog::debug("Error: {}", SDL_GetError());
        SDL_Log("Failed to open audio: %s", SDL_GetError());
    }
}

Chip8Audio::~Chip8Audio()
{
    if (device_ != 0)
        SDL_CloseAudioDevice(device_);
}

void Chip8Audio::processAudio(const chip8core::Chip8Timer& soundTimer)
{
    if (soundTimer.getValue() > 0)
    {
        spdlog::debug("BEEP");
        play();
    }
    else
    {
        stop();
    }
}

void Chip8Audio::play()
{

    playing_ = true;
    if (device_ != 0)
        SDL_PauseAudioDevice(device_, 0);
}

void Chip8Audio::stop()
{
    playing_ = false;
    if (device_ != 0)
        SDL_PauseAudioDevice(device_, 1);
}

void Chip8Audio::audioCallback(void* userdata, Uint8* stream, int len)
{
    auto*  self            = static_cast<Chip8Audio*>(userdata);
    Uint8* buf             = stream;
    int    freq            = 440; // Square wave frequency (Hz)
    int    samplesPerCycle = self->spec_.freq / freq;

    for (int i = 0; i < len; ++i)
    {
        if (self->playing_)
        {
            buf[i] = (self->phase_ < samplesPerCycle / 2) ? 0xFF : 0x00; // Square wave
        }
        else
        {
            buf[i] = 0x80; // Silence (center for AUDIO_U8)
        }
        self->phase_ = (self->phase_ + 1) % samplesPerCycle;
    }
}