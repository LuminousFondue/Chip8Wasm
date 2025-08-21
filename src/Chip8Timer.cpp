#include "Chip8Timer.h"

Chip8Timer::Chip8Timer() : currentValue_(0)
{
}

Chip8Timer::~Chip8Timer()
{
}

void Chip8Timer::update()
{
    if (currentValue_ > 0)
    {
        --currentValue_;
    }
}

bool Chip8Timer::isActive()
{
    return (currentValue_ > 0);
}
