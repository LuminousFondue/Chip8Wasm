#pragma once

#include "Chip8Memory.h"

class Chip8CPU
{
  public:
    /**
     * @brief Constructs a Chip8CPU instance.
     * @param memory Reference to the Chip8Memory instance.
     */
    explicit Chip8CPU(Chip8Memory& memory);

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

#ifdef UNIT_TEST
    // Expose private members for testing only
    uint16_t& PC() { return PC_; }
    uint8_t&  SP() { return SP_; }
    uint16_t& I() { return I_; }
    uint8_t*  V() { return V_; }
#endif

  private:
    uint8_t      V_[16];     // General purpose registers
    uint16_t     I_;         // Index register
    uint16_t     PC_;        // Program counter
    uint8_t      SP_;        // Stack pointer
    uint16_t     stack_[16]; // Stack for subroutine calls
    Chip8Memory& memory_;
};