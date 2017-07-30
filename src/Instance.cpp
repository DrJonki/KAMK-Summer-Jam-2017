#include <Jam/Instance.hpp>
#include <Jam/Scene.hpp>
#include <SFML/Window/Event.hpp>
#include <rapidjson/document.h>

sf::VideoMode getVideomode(const jam::ConfigManager& config) {
  #ifdef _DEBUG
    return sf::VideoMode(config.integer("VIEW_X"), config.integer("VIEW_Y"));
  #else
    return sf::VideoMode::getDesktopMode();
  #endif
}

sf::Uint32 getStyle() {
  #ifdef _DEBUG
    return sf::Style::Default;
  #else
    return sf::Style::None;
  #endif
}

namespace jam
{
  Instance::Instance()
    : config(),
      window(getVideomode(config), "Jam", getStyle()),
      currentScene(),
      resourceManager(),
      highscores(),
      m_clock(),
      apiKey("iKMewtMiDYmshbuIQbFJc0kZGN4Mp1ecPCsjsnJwzCOSEph84a")
  {
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);



    http.setHost("http://kvstore.p.mashape.com");

    auto req = sf::Http::Request("/collections/scores/items?limit=10&sort=desc");
    req.setField("X-Mashape-Key", apiKey);
    req.setField("Accept", "application/json");

    sf::Http::Response res = http.sendRequest(req);
    if (res.getStatus() == sf::Http::Response::Ok) {
      rapidjson::Document doc;
      doc.Parse<0>(res.getBody().c_str());

      if (!doc.HasParseError()) {
        for (auto itr = doc.Begin(); itr != doc.End(); ++itr);
      }
    }
  }

  Instance::~Instance()
  {}

  void Instance::operator ()()
  {
    if (currentScene)
      currentScene->update(
        m_clock.restart().asSeconds() *
        config.float_("SPEED_MULT") // Global game speed multiplier
      );

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
      case sf::Event::Resized:
      {
        const auto view = window.getView().getSize();
        const auto ratio = view.x / view.y;
        window.setSize(sf::Vector2u(
          static_cast<unsigned int>(window.getSize().y * ratio),
          window.getSize().y
        ));
        break;
      }
      case sf::Event::KeyReleased:
      {
        if (event.key.code == sf::Keyboard::Escape)
          window.close();

        break;
      }
      case sf::Event::TextEntered:
      {
        currentScene->textEvent(event.text.unicode);
        break;
      }
      }
    }
  }
}

