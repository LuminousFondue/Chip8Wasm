#include "Chip8Core/Chip8Memory.h"

#include <spdlog/spdlog.h>

#include <cstdio>
#include <cstring>
namespace chip8core
{
Chip8Memory::Chip8Memory()
{
    initialize();
    spdlog::debug("Chip8 Memory created");
}

Chip8Memory::~Chip8Memory()
{
    spdlog::debug("Chip8 Memory destroyed");
}

void Chip8Memory::clear()
{
    std::memset(memory_, 0, MEMORY_SIZE);
    spdlog::debug("Chip8 Memory cleared");
}

void Chip8Memory::initialize()
{
    spdlog::debug("Initializing Chip8 Memory");
    clear();
}

void Chip8Memory::write(uint16_t address, uint8_t value)
{
    if (address >= MEMORY_SIZE)
    {
        spdlog::critical("Attempted to write to out of bounds address 0x{:x}", address);
        throw Chip8MemoryException(Chip8MemoryException::WRITE_OUT_OF_BOUNDS);
    }
    memory_[address] = value;
    spdlog::trace("Wrote value 0x{:x} to address 0x{:x}", value, address);
}

void Chip8Memory::write(uint16_t address, const std::vector<uint8_t>& data)
{
    if (address + data.size() > MEMORY_SIZE || address >= MEMORY_SIZE)
    {
        spdlog::critical("Attempted to write block to out of bounds address 0x{:x} (length: {})",
                         address, data.size());
        throw Chip8MemoryException(Chip8MemoryException::WRITE_OUT_OF_BOUNDS);
    }
    std::memcpy(memory_ + address, data.data(), data.size());
    spdlog::trace("Wrote {} bytes to address 0x{:x}", data.size(), address);
}

uint8_t Chip8Memory::read(uint16_t address) const
{
    if (address >= MEMORY_SIZE)
    {
        spdlog::critical("Attempted to read from out of bounds address 0x{:x}", address);
        throw Chip8MemoryException(Chip8MemoryException::READ_OUT_OF_BOUNDS);
    }
    return memory_[address];
}

std::vector<uint8_t> Chip8Memory::read(uint16_t address, size_t length) const
{
    if (address + length > MEMORY_SIZE || address >= MEMORY_SIZE)
    {
        spdlog::critical("Attempted to read block from out of bounds address 0x{:x} (length: {})",
                         address, length);
        throw Chip8MemoryException(Chip8MemoryException::READ_OUT_OF_BOUNDS);
    }
    return std::vector<uint8_t>(memory_ + address, memory_ + address + length);
}

std::vector<uint8_t> Chip8Memory::dump() const
{
    return std::vector<uint8_t>(memory_, memory_ + MEMORY_SIZE);
}
} // namespace chip8core