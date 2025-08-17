#pragma once
#include <cstdint>
#include <stdexcept>
#include <vector>

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
    static constexpr uint16_t MEMORY_SIZE = 4096; // 4KB of memory

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
     * @brief Reads a block of memory from the specified address.
     * @param address The starting memory address to read from.
     * @param length The number of bytes to read.
     * @return A vector containing the read bytes.
     * @throws Chip8MemoryException if the address or length is out of bounds.
     */
    std::vector<uint8_t> read(uint16_t address, size_t length) const;

    /**
     * @brief Writes a byte to the specified memory address.
     * @param address The memory address to write to.
     * @param value The byte value to write.
     * @throws Chip8MemoryException if the address is out of bounds.
     */
    void write(uint16_t address, uint8_t value);

    /**
     * @brief Writes a block of memory to the specified address.
     * @param address The memory address to write to.
     * @param data The byte array to write.
     * @throws Chip8MemoryException if the address is out of bounds.
     */
    void write(uint16_t address, const std::vector<uint8_t>& data);

    /**
     * @brief Clears all memory by setting every byte to zero.
     */
    void clear();

    /**
     * @brief Dumps the entire memory contents as a vector.
     * @return A vector containing all bytes in memory.
     */
    std::vector<uint8_t> dump() const;

  private:
    uint8_t memory_[MEMORY_SIZE];

    /**
     * @brief Initializes the Chip8 memory.
     */
    void initialize();
};