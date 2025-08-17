#include "Chip8.h"

Chip8::Chip8() : memory_(), cpu_(memory_)
{
    printf("Chip8 created.\n");
}

Chip8::~Chip8()
{
    printf("Chip8 destroyed\n");
}