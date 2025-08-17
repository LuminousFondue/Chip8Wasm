#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>

/**
 * @brief Exception class for Chip8Memory errors.
 */
class Chip8MemoryException : public std::runtime_error
{
  public:
    static constexpr const char* READ_OUT_OF_BOUNDS  = "Read out of bounds";
    static constexpr const char* WRITE_OUT_OF_BOUNDS = "Write out of bounds";

    explicit Chip8MemoryException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * @brief Represents the 4KB memory of a Chip-8 system.
 */
class Chip8Memory
{
  public:
    static constexpr uint16_t MEMORY_SIZE = 4096;

    /**
     * @brief Constructs and initializes the Chip8 memory.
     */
    Chip8Memory();

    /**
     * @brief Destructor for Chip8Memory.
     */
    ~Chip8Memory();

    /**
     * @brief Reads a byte from the specified memory address.
     * @param address The memory address to read from.
     * @return The byte value at the given address.
     * @throws Chip8MemoryException if the address is out of bounds.
     */
    uint8_t read(uint16_t address) const;

    /**
     * @brief Writes a byte to the specified memory address.
     * @param address The memory address to write to.
     * @param value The byte value to write.
     * @throws Chip8MemoryException if the address is out of bounds.
     */
    void write(uint16_t address, uint8_t value);

    /**
     * @brief Clears all memory by setting every byte to zero.
     */
    void clear();

  private:
    uint8_t memory_[MEMORY_SIZE];

    /**
     * @brief Initializes the Chip8 memory.
     *        Currently, this just clears the memory.
     */
    void initialize();
};