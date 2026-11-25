#include "chip8.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

constexpr unsigned int CHIP_8_WIDTH = 64;
constexpr unsigned int CHIP_8_HEIGHT = 32;
constexpr float SCALE = 16.f;

auto map_key = [](sf::Keyboard::Key key) -> std::optional<uint8_t> {
    switch (key) {
    case sf::Keyboard::Key::Num1:
        return 0x1;
    case sf::Keyboard::Key::Num2:
        return 0x2;
    case sf::Keyboard::Key::Num3:
        return 0x3;
    case sf::Keyboard::Key::Num4:
        return 0xC;

    case sf::Keyboard::Key::Q:
        return 0x4;
    case sf::Keyboard::Key::W:
        return 0x5;
    case sf::Keyboard::Key::E:
        return 0x6;
    case sf::Keyboard::Key::R:
        return 0xD;

    case sf::Keyboard::Key::A:
        return 0x7;
    case sf::Keyboard::Key::S:
        return 0x8;
    case sf::Keyboard::Key::D:
        return 0x9;
    case sf::Keyboard::Key::F:
        return 0xE;

    case sf::Keyboard::Key::Y:
        return 0xA;
    case sf::Keyboard::Key::X:
        return 0x0;
    case sf::Keyboard::Key::C:
        return 0xB;
    case sf::Keyboard::Key::V:
        return 0xF;

    default:
        return std::nullopt;
    }
};

int main(int argc, char *argv[]) {
    sf::RenderWindow window(sf::VideoMode({1024, 512}), "CHIP-8 Emulator",
                            sf::Style::Titlebar | sf::Style::Close);

    window.setFramerateLimit(60);

    std::string rom_path = "roms/IBM_Logo.ch8";

    if (argc >= 2) {
        rom_path = argv[1];
    } else {
        std::cout << "Kein ROM angegeben, nutze Standard: " << rom_path << '\n';
    }

    Chip8 chip8;

    if (!chip8.load_rom(rom_path)) {
        return 1;
    }

    sf::Texture texture(sf::Vector2u(CHIP_8_WIDTH, CHIP_8_HEIGHT));
    texture.setSmooth(false);

    sf::Sprite sprite(texture);
    sprite.setScale(sf::Vector2f(SCALE, SCALE));
    std::vector<uint8_t> pixels(CHIP_8_WIDTH * CHIP_8_HEIGHT * 4, 0);

    constexpr float TIMER_INTERVAL = 1.f / 60.f;
    sf::Clock clock;
    float timer_accumulator = 0.f;
    constexpr size_t CYCLES_PER_FRAME = 10;

    while (window.isOpen()) {
        const float dt = clock.restart().asSeconds();
        timer_accumulator += dt;

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto *key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                if (const auto chip8_key = map_key(key_pressed->code)) {
                    chip8.set_key(*chip8_key, true);
                }
            } else if (const auto *key_released = event->getIf<sf::Event::KeyReleased>()) {
                if (const auto chip8_key = map_key(key_released->code)) {
                    chip8.set_key(*chip8_key, false);
                }
            }
        }

        for (size_t i = 0; i < CYCLES_PER_FRAME; ++i) {
            chip8.cycle();
        }

        while (timer_accumulator >= TIMER_INTERVAL) {
            chip8.update_timers();
            timer_accumulator -= TIMER_INTERVAL;
        }

        const std::array<uint8_t, 64 * 32> &display = chip8.get_display();

        for (size_t i = 0; i < display.size(); ++i) {
            const bool pixel_on = display[i];
            const uint8_t color = pixel_on ? 255 : 0;

            const size_t idx = i * 4;
            pixels[idx + 0] = color; // R
            pixels[idx + 1] = color; // G
            pixels[idx + 2] = color; // B
            pixels[idx + 3] = 255;   // A
        }

        texture.update(pixels.data());

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
