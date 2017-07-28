#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>

namespace jam
{
  class Scene;

  class Instance final
  {
  public:

    Instance();

    ~Instance();

    void loop();

  public:

    // Globals
    sf::RenderWindow window;
    std::unique_ptr<Scene> currentScene;

  private:

    sf::Clock m_clock;
  };
}