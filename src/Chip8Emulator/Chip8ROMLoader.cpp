#include "Chip8Emulator/Chip8ROMLoader.h"

#include <spdlog/spdlog.h>

#include <fstream>
#include <vector>

bool Chip8ROMLoader::loadROM(const std::string& filename, chip8core::Chip8& chip8)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file)
    {
        spdlog::error("Failed to open ROM file: {}", filename);
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        spdlog::error("Failed to read ROM file: {}", filename);
        return false;
    }
    chip8.loadROM(buffer.data(), buffer.size());
    spdlog::info("Loaded ROM: {} ({} bytes)", filename, size);
    return true;
}