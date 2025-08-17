#include "Chip8Memory.h"

Chip8Memory::Chip8Memory()
{
    initialize();
    printf("Chip8 Memory created.\n");
}

Chip8Memory::~Chip8Memory()
{
    printf("Chip8 Memory destroyed.\n");
}

void Chip8Memory::clear()
{
    std::memset(memory_, 0, MEMORY_SIZE);
    printf("Chip8 Memory cleared.\n");
}

void Chip8Memory::initialize()
{
    printf("Initializing Chip8 Memory\n");
    clear();
}

void Chip8Memory::write(uint16_t address, uint8_t value)
{
    if (address >= MEMORY_SIZE)
    {
        throw Chip8MemoryException(Chip8MemoryException::WRITE_OUT_OF_BOUNDS);
    }
    memory_[address] = value;
    printf("Wrote value 0x%x to address 0x%x\n", value, address);
}

uint8_t Chip8Memory::read(uint16_t address) const
{
    if (address >= MEMORY_SIZE)
    {
        throw Chip8MemoryException(Chip8MemoryException::READ_OUT_OF_BOUNDS);
    }
    return memory_[address];
}