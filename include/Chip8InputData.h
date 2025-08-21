#pragma once

class Chip8InputData
{
  public:
    Chip8InputData();
    ~Chip8InputData();

    void setKeyState(int key, bool pressed);
    bool isKeyPressed(int key) const;
    bool wasKeyReleased(int key) const;

  private:
    bool keyStates[16];
    bool previousKeyStates[16];

    void storeKeyStates();
};