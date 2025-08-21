#include "Chip8InputData.h"

Chip8InputData::Chip8InputData()
{
    for (int i = 0; i < 16; ++i)
    {
        keyStates[i] = false;
    }
}

Chip8InputData::~Chip8InputData() = default;

void Chip8InputData::setKeyState(int key, bool pressed)
{
    if (key >= 0 && key < 16)
    {
        keyStates[key] = pressed;
    }
}

bool Chip8InputData::isKeyPressed(int key) const
{
    if (key >= 0 && key < 16)
    {
        return keyStates[key];
    }
    return false;
}

void Chip8InputData::storeKeyStates()
{
    for (int i = 0; i < 16; ++i)
    {
        previousKeyStates[i] = keyStates[i];
    }
}

bool Chip8InputData::wasKeyReleased(int key) const
{
    if (key >= 0 && key < 16)
    {
        return previousKeyStates[key] && !keyStates[key];
    }
    return false;
}