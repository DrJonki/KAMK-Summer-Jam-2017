#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

namespace jam
{
  class Scene;

  class Instance final
  {
  public:

    Instance();

    ~Instance();

    void draw();

    void handleEvents();

  public:

    // Globals
    sf::RenderWindow window;
    std::unique_ptr<Scene> currentScene;
  };
}