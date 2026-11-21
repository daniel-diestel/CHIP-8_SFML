#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({512, 256}), "CHIP-8 Emulator",
                            sf::Style::Titlebar | sf::Style::Close);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        // window.draw();
        window.display();
    }
}
