#include <gtest/gtest.h>

#include "Chip8GraphicsBuffer.h"

TEST(Chip8GraphicsBufferTests, ClearFramebuffer)
{
    Chip8GraphicsBuffer graphics;
    graphics.setPixel(10, 10, true);
    graphics.clear();
    EXPECT_FALSE(graphics.getPixel(10, 10));
}

TEST(Chip8GraphicsBufferTests, SetGetPixel)
{
    Chip8GraphicsBuffer graphics;
    graphics.setPixel(10, 10, true);
    EXPECT_TRUE(graphics.getPixel(10, 10));
    graphics.setPixel(10, 10, false);
    EXPECT_FALSE(graphics.getPixel(10, 10));
}

TEST(Chip8GraphicsBufferTests, OutOfBoundsSetPixel)
{
    Chip8GraphicsBuffer graphics;
    EXPECT_THROW(graphics.setPixel(-1, 0, true), Chip8GraphicsError);
    EXPECT_THROW(graphics.setPixel(0, -1, true), Chip8GraphicsError);
    EXPECT_THROW(graphics.setPixel(Chip8GraphicsBuffer::FRAMEBUFFER_WIDTH, 0, true),
                 Chip8GraphicsError);
    EXPECT_THROW(graphics.setPixel(0, Chip8GraphicsBuffer::FRAMEBUFFER_HEIGHT, true),
                 Chip8GraphicsError);
}

TEST(Chip8GraphicsBufferTests, OutOfBoundsGetPixel)
{
    Chip8GraphicsBuffer graphics;
    EXPECT_THROW(graphics.getPixel(-1, 0), Chip8GraphicsError);
    EXPECT_THROW(graphics.getPixel(0, -1), Chip8GraphicsError);
    EXPECT_THROW(graphics.getPixel(Chip8GraphicsBuffer::FRAMEBUFFER_WIDTH, 0), Chip8GraphicsError);
    EXPECT_THROW(graphics.getPixel(0, Chip8GraphicsBuffer::FRAMEBUFFER_HEIGHT), Chip8GraphicsError);
}
