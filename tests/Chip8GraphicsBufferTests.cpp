#include <gtest/gtest.h>

#include "Chip8Core/Chip8GraphicsBuffer.h"

TEST(Chip8GraphicsBufferTests, ClearFramebuffer)
{
    chip8core::Chip8GraphicsBuffer graphics;
    graphics.setPixel(10, 10, true);
    graphics.clear();
    EXPECT_FALSE(graphics.getPixel(10, 10));
}

TEST(Chip8GraphicsBufferTests, SetGetPixel)
{
    chip8core::Chip8GraphicsBuffer graphics;
    graphics.setPixel(10, 10, true);
    EXPECT_TRUE(graphics.getPixel(10, 10));
    graphics.setPixel(10, 10, false);
    EXPECT_FALSE(graphics.getPixel(10, 10));
}

TEST(Chip8GraphicsBufferTests, OutOfBoundsSetPixel)
{
    chip8core::Chip8GraphicsBuffer graphics;
    EXPECT_THROW(graphics.setPixel(-1, 0, true), chip8core::Chip8GraphicsError);
    EXPECT_THROW(graphics.setPixel(0, -1, true), chip8core::Chip8GraphicsError);
    EXPECT_THROW(graphics.setPixel(chip8core::Chip8GraphicsBuffer::FRAMEBUFFER_WIDTH, 0, true),
                 chip8core::Chip8GraphicsError);
    EXPECT_THROW(graphics.setPixel(0, chip8core::Chip8GraphicsBuffer::FRAMEBUFFER_HEIGHT, true),
                 chip8core::Chip8GraphicsError);
}

TEST(Chip8GraphicsBufferTests, OutOfBoundsGetPixel)
{
    chip8core::Chip8GraphicsBuffer graphics;
    EXPECT_THROW(graphics.getPixel(-1, 0), chip8core::Chip8GraphicsError);
    EXPECT_THROW(graphics.getPixel(0, -1), chip8core::Chip8GraphicsError);
    EXPECT_THROW(graphics.getPixel(chip8core::Chip8GraphicsBuffer::FRAMEBUFFER_WIDTH, 0),
                 chip8core::Chip8GraphicsError);
    EXPECT_THROW(graphics.getPixel(0, chip8core::Chip8GraphicsBuffer::FRAMEBUFFER_HEIGHT),
                 chip8core::Chip8GraphicsError);
}
