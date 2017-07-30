#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <Jam/ResourceManager.hpp>
#include <Jam/ConfigManager.hpp>
#include <set>

namespace jam
{
  class Scene;

  class Instance final
  {
    Instance(const Instance&) = delete;
    void operator =(const Instance&) = delete;

  public:

    Instance();

    ~Instance();

    void operator ()();

    bool sendRequest(const std::string& url, std::string& res);

    bool sendPutRequest(const std::string& url, const std::string& body);

  public:

    // Globals
    ConfigManager config;
    sf::RenderWindow window;
    std::unique_ptr<Scene> currentScene;
    ResourceManager resourceManager;
    std::map<sf::String, int> highscores;

  public:

    sf::Clock m_clock;
  };
}