#include "Chip8InputBuffer.h"

Chip8InputBuffer::Chip8InputBuffer()
{
    for (int i = 0; i < 16; ++i)
    {
        keyStates[i] = false;
    }
}

Chip8InputBuffer::~Chip8InputBuffer() = default;

void Chip8InputBuffer::setKeyState(int key, bool pressed)
{
    if (key >= 0 && key < 16)
    {
        keyStates[key] = pressed;
    }
}

bool Chip8InputBuffer::isKeyPressed(int key) const
{
    if (key >= 0 && key < 16)
    {
        return keyStates[key];
    }
    return false;
}