#pragma once

class Chip8InputData
{
  public:
    Chip8InputData();
    ~Chip8InputData();

    void setKeyState(int key, bool pressed);
    bool isKeyPressed(int key) const;

  private:
    bool keyStates[16];
};