#pragma once
#include <array>

#include "Chip8GraphicsData.h"
#include "Chip8Memory.h"

class Chip8CPU
{
  public:
    /**
     * @brief Constructs a Chip8CPU instance.
     * @param memory Reference to the Chip8Memory instance.
     * @param graphics Reference to the Chip8GraphicsData instance.
     */
    explicit Chip8CPU(Chip8Memory& memory, Chip8GraphicsData& graphics);

    /**
     * @brief Destroys the Chip8CPU instance.
     */
    ~Chip8CPU();

    /**
     * @brief Executes a single CPU cycle.
     */
    void cycle();

    /**
     * @brief Resets the CPU to its initial state.
     */
    void reset();

    /**
     * @brief Gets the current program counter.
     */
    uint16_t getPC() const { return PC_; }

    /**
     * @brief Gets the current stack pointer.
     */
    uint16_t getSP() const { return SP_; }

    /**
     * @brief Gets the current index register.
     */
    uint16_t getI() const { return I_; }

    /**
     * @brief Gets the value of a specific register.
     * @param index The index of the register (0-15).
     * @return The value of the specified register.
     */
    uint8_t getV(size_t index) const
    {
        if (index < 16)
        {
            return V_[index];
        }
        throw std::out_of_range("Invalid register index");
    }

    /**
     * @brief Sets the value of a specific register.
     * @param index The index of the register (0-15).
     * @param value The value to set.
     */
    void setV(size_t index, uint8_t value)
    {
        if (index < 16)
        {
            V_[index] = value;
            return;
        }
        throw std::out_of_range("Invalid register index");
    }

  private:
    uint8_t  V_[16];     // General purpose registers
    uint16_t I_;         // Index register
    uint16_t PC_;        // Program counter
    uint8_t  SP_;        // Stack pointer
    uint16_t stack_[16]; // Stack for subroutine calls

    Chip8Memory&       memory_;
    Chip8GraphicsData& graphics_;

    using OpcodeHandler = void (Chip8CPU::*)(uint16_t);

    std::array<OpcodeHandler, 16>   main_opcode_table_;
    std::array<OpcodeHandler, 16>   _0_table;
    std::array<OpcodeHandler, 16>   _8_table;
    std::array<OpcodeHandler, 16>   _E_table;
    std::array<OpcodeHandler, 0x66> _F_table;

    void decodeOpcode(uint16_t opcode);
    void invalidOpcode(uint16_t opcode);
    void initializeOpcodeTables();

    static uint8_t getNibble(uint16_t opcode, int nibbleIndex);

    void handle_0XXX(uint16_t opcode);
    void handle_8XXX(uint16_t opcode);
    void handle_EXXX(uint16_t opcode);
    void handle_FXXX(uint16_t opcode);

    void opcode_00E0(uint16_t opcode);
    void opcode_00EE(uint16_t opcode);
    void opcode_1NNN(uint16_t opcode);
    void opcode_2NNN(uint16_t opcode);
    void opcode_3XKK(uint16_t opcode);
    void opcode_4XKK(uint16_t opcode);
    void opcode_5XY0(uint16_t opcode);
    void opcode_6XKK(uint16_t opcode);
    void opcode_7XKK(uint16_t opcode);
    void opcode_8XY0(uint16_t opcode);
    void opcode_8XY1(uint16_t opcode);
    void opcode_8XY2(uint16_t opcode);
    void opcode_8XY3(uint16_t opcode);
    void opcode_8XY4(uint16_t opcode);
    void opcode_8XY5(uint16_t opcode);
    void opcode_8XY6(uint16_t opcode);
    void opcode_8XY7(uint16_t opcode);
    void opcode_8XYE(uint16_t opcode);
    void opcode_9XY0(uint16_t opcode);
    void opcode_ANNN(uint16_t opcode);
    void opcode_BNNN(uint16_t opcode);
    void opcode_CXKK(uint16_t opcode);
    void opcode_DXYN(uint16_t opcode);
    void opcode_EXA1(uint16_t opcode);
    void opcode_EX9E(uint16_t opcode);
    void opcode_FX07(uint16_t opcode);
    void opcode_FX0A(uint16_t opcode);
    void opcode_FX15(uint16_t opcode);
    void opcode_FX18(uint16_t opcode);
    void opcode_FX1E(uint16_t opcode);
    void opcode_FX29(uint16_t opcode);
    void opcode_FX33(uint16_t opcode);
    void opcode_FX55(uint16_t opcode);
    void opcode_FX65(uint16_t opcode);
};