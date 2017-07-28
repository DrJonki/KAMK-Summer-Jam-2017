#include <Jam/Instance.hpp>
#include <Jam/Scene.hpp>
#include <SFML/Window/Event.hpp>

namespace jam
{
  Instance::Instance()
    : window(sf::VideoMode(800, 600), "Jam")
  {


    m_clock.restart(); // Remove initialization time
  }

  Instance::~Instance()
  {}

  void Instance::loop()
  {
    if (currentScene)
      currentScene->update(m_clock.restart().asSeconds());

    window.clear();

    if (currentScene)
      currentScene->draw(window);

    window.display();

    // Handle events
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

