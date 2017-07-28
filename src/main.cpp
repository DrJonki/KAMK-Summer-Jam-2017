#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Jam");

  while (window.isOpen()) {
    // Rendering
    window.clear(sf::Color::Blue);
    window.display();

    // Event loop
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;
      }
    }
  }

  return 0;
}