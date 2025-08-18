#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>

/**
 * Custom exception class for Chip8 graphics errors.
 */
class Chip8GraphicsError : public std::runtime_error
{
  public:
    static constexpr const char* OUT_OF_BOUNDS = "Graphics operation out of bounds";
    explicit Chip8GraphicsError(const std::string& message) : std::runtime_error(message) {}
};

/**
 * Class representing the graphics data for the Chip8 emulator.
 */
class Chip8GraphicsData
{
  public:
    static constexpr int FRAMEBUFFER_WIDTH  = 64;
    static constexpr int FRAMEBUFFER_HEIGHT = 32;

    Chip8GraphicsData();
    ~Chip8GraphicsData();

    /**
     * Clears the framebuffer.
     */
    void clear();

    /**
     * Sets a pixel in the framebuffer.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param value The value to set (true for on, false for off).
     */
    void setPixel(int x, int y, bool value);

    /**
     * Gets the value of a pixel in the framebuffer.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The value of the pixel (true for on, false for off).
     */
    bool getPixel(int x, int y) const;

    std::vector<uint32_t> dumpFrameBuffer() const;

  private:
    uint32_t framebuffer_[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
};