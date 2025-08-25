#pragma once
#include <cstdint>
namespace chip8core
{

class Chip8Timer
{
  public:
    Chip8Timer();
    ~Chip8Timer();

    void    update();
    void    reset() { currentValue_ = 0; }
    void    setValue(uint8_t value) { currentValue_ = value; }
    uint8_t getValue() const { return currentValue_; }
    bool    isActive();

  private:
    uint8_t currentValue_;
};
} // namespace chip8core