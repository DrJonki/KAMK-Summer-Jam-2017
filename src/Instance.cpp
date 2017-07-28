#include <Jam/Instance.hpp>
#include <Jam/Scene.hpp>
#include <SFML/Window/Event.hpp>

namespace jam
{
  Instance::Instance()
    : window(sf::VideoMode(800, 600), "Jam")
  {

  }

  Instance::~Instance()
  {}

  void Instance::draw()
  {
    window.clear();

    if (currentScene)
      currentScene->draw(window);

    window.display();
  }

  void Instance::handleEvents()
  {
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
}

