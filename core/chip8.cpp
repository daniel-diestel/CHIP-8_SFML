#include "chip8.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

namespace {
constexpr size_t ROM_START_ADDRESS = 0x200;
constexpr size_t FONT_START_ADDRESS = 0x050;

constexpr std::array<uint8_t, 5 * 16> fontset{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
} // namespace

Chip8::Chip8() { reset(); }

void Chip8::set_key(uint8_t key, bool pressed) {
    if (key < keypad.size()) {
        keypad[key] = pressed;
    }
}

const std::array<uint8_t, 64 * 32> &Chip8::get_display() const { return gfx; }

bool Chip8::is_sound_playing() const { return s_timer > 0; }

void Chip8::update_timers() {
    if (d_timer > 0) {
        --d_timer;
    }
    if (s_timer > 0) {
        --s_timer;
    }
}

void Chip8::reset() {
    memory.fill(0);
    V.fill(0);
    pc = ROM_START_ADDRESS;
    I = 0;
    stack.fill(0);
    sp = 0;
    d_timer = 0;
    s_timer = 0;
    gfx.fill(0);
    keypad.fill(false);

    for (size_t i{}; i < fontset.size(); ++i) {
        memory[FONT_START_ADDRESS + i] = fontset[i];
    }
}

bool Chip8::load_rom(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open ROM file: " << filename << '\n';
        return false;
    }

    const auto file_size = file.tellg();
    if (file_size <= 0) {
        std::cerr << "ROM file is empty or invalid size: " << filename << '\n';
        return false;
    }

    const size_t rom_size = static_cast<size_t>(file_size);
    if (rom_size > memory.size() - ROM_START_ADDRESS) {
        std::cerr << "ROM is too large to fit in memory\n";
        return false;
    }

    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(rom_size);
    if (!file.read(buffer.data(), static_cast<std::streamsize>(rom_size))) {
        std::cerr << "Failed to read ROM file: " << filename << '\n';
        return false;
    }

    for (size_t i = 0; i < rom_size; ++i) {
        memory[ROM_START_ADDRESS + i] = static_cast<uint8_t>(buffer[i]);
    }

    return true;
}

void Chip8::cycle() {
    const uint16_t opcode = static_cast<uint16_t>((static_cast<uint16_t>(memory[pc]) << 8) |
                                                  static_cast<uint8_t>(memory[pc + 1]));

    pc += 2;

    execute_opcode(opcode);
}

void Chip8::execute_opcode(uint16_t opcode) {}
