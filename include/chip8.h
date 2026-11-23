#ifndef CHIP_8_H
#define CHIP_8_H

#include <array>
#include <cstdint>
#include <string>

class Chip8 {
  private:
    std::array<uint8_t, 4096> memory{}; // RAM
    std::array<uint8_t, 16> V{};        // general purpose registers
    uint16_t pc{0x200};                 // program counter
    uint16_t I{};                       // index register
    std::array<uint16_t, 16> stack{};   // stack
    uint8_t sp{};                       // stack pointer
    uint8_t d_timer{};                  // delay timer
    uint8_t s_timer{};                  // sound timer
    std::array<uint8_t, 64 * 32> gfx{}; // display
    std::array<bool, 16> keypad{};      // keypad

    void execute_opcode(uint16_t opcode);

  public:
    Chip8();

    void reset();
    bool load_rom(const std::string &filename);
    void cycle();
    void update_timers();

    void set_key(uint8_t key, bool pressed);
    const std::array<uint8_t, 64 * 32> &get_display() const;
    bool is_sound_playing() const;
};

#endif
