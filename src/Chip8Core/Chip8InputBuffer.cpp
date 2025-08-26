#include "Chip8Core/Chip8InputBuffer.h"

namespace chip8core
{
Chip8InputBuffer::Chip8InputBuffer()
{
    for (int i = 0; i < 16; ++i)
    {
        keyStates[i] = false;
    }
}

Chip8InputBuffer::~Chip8InputBuffer() = default;

void Chip8InputBuffer::syncKeyStates()
{
    for (int i = 0; i < 16; ++i)
        prevKeyStates[i] = keyStates[i];
}

void Chip8InputBuffer::setKeyState(uint8_t key, bool pressed)
{
    if (key >= 0 && key < 16)
    {
        keyStates[key] = pressed;
    }
}

bool Chip8InputBuffer::getKeyState(uint8_t key) const
{

    if (key >= 0 && key < 16)
    {
        return keyStates[key];
    }
    return false;
}

bool Chip8InputBuffer::wasKeyReleased(uint8_t key) const
{
    if (key < 16)
        return prevKeyStates[key] && !keyStates[key];
    return false;
}
} // namespace chip8core