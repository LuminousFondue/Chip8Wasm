#pragma once

class Chip8InputBuffer
{
  public:
    Chip8InputBuffer();
    ~Chip8InputBuffer();

    void setKeyState(int key, bool pressed);
    bool isKeyPressed(int key) const;

  private:
    bool keyStates[16];
};