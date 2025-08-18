#include "Chip8GraphicsData.h"

#include <cstring>

Chip8GraphicsData::Chip8GraphicsData()
{
    clear();
}

Chip8GraphicsData::~Chip8GraphicsData()
{
}

void Chip8GraphicsData::clear()
{
    std::memset(framebuffer_, 0, sizeof(framebuffer_));
}

void Chip8GraphicsData::setPixel(int x, int y, bool value)
{
    if (x < 0 || x >= FRAMEBUFFER_WIDTH || y < 0 || y >= FRAMEBUFFER_HEIGHT)
    {
        throw Chip8GraphicsError(Chip8GraphicsError::OUT_OF_BOUNDS);
    }

    if (value)
        framebuffer_[y * FRAMEBUFFER_WIDTH + x] |= 0xFFFFFFFF;
    else
        framebuffer_[y * FRAMEBUFFER_WIDTH + x] &= 0x00000000;
}

bool Chip8GraphicsData::getPixel(int x, int y) const
{
    if (x < 0 || x >= FRAMEBUFFER_WIDTH || y < 0 || y >= FRAMEBUFFER_HEIGHT)
    {
        throw Chip8GraphicsError(Chip8GraphicsError::OUT_OF_BOUNDS);
    }

    return framebuffer_[y * FRAMEBUFFER_WIDTH + x] != 0;
}

std::vector<uint32_t> Chip8GraphicsData::dumpFrameBuffer() const
{
    return std::vector<uint32_t>(framebuffer_,
                                 framebuffer_ + sizeof(framebuffer_) / sizeof(uint32_t));
}