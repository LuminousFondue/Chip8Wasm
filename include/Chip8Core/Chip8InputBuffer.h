#pragma once
#include <cstdint>

namespace chip8core
{
class Chip8InputBuffer
{
  public:
    Chip8InputBuffer();
    ~Chip8InputBuffer();

    void syncKeyStates();
    void setKeyState(uint8_t key, bool pressed);
    bool getKeyState(uint8_t key) const;
    bool wasKeyReleased(uint8_t key) const;

  private:
    bool keyStates[16];
    bool prevKeyStates[16];
};
} // namespace chip8core