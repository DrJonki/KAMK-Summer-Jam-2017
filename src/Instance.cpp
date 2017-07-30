#include <Jam/Instance.hpp>
#include <Jam/Scene.hpp>
#include <SFML/Window/Event.hpp>
#include <rapidjson/document.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

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

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

namespace jam
{
  Instance::Instance()
    : config(),
      window(getVideomode(config), "Jam", getStyle()),
      currentScene(),
      resourceManager(),
      highscores(),
      m_clock()
  {
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    std::string res;
    const bool success = sendRequest("https://kvstore.p.mashape.com/collections/scores/items?limit=10&sort=desc", res);

    if (success) {
      rapidjson::Document doc;
      doc.Parse<0>(res.c_str());
      int amount = 0;

      if (!doc.HasParseError() && doc.IsArray()) {
        for (auto itr = doc.Begin(); itr != doc.End() && amount <= 10; ++itr) {
          const auto key = std::string((*itr)["key"].GetString());
          highscores.emplace(sf::String::fromUtf8(key.begin(), key.end()), std::atoi((*itr)["value"].GetString()));
          ++amount;
        }
      }
    }
  }

  Instance::~Instance()
  {
    curl_global_cleanup();
  }

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
      case sf::Event::JoystickButtonPressed:
      {
        std::cout << "Button: " << event.joystickButton.button << std::endl;
        break;
      }
      case sf::Event::JoystickMoved:
      {
        std::cout << "Axis: " << event.joystickMove.axis << ": " << event.joystickMove.position << std::endl;
        break;
      }
      }
    }
  }

  bool Instance::sendRequest(const std::string& url, std::string& res)
  {
    CURL *curl;
    CURLcode response;

    bool success = false;

    curl = curl_easy_init();
    if (curl) {
      struct curl_slist *chunk = NULL;

      chunk = curl_slist_append(chunk, "Accept: application/json");
      chunk = curl_slist_append(chunk, "X-Mashape-Key: iKMewtMiDYmshbuIQbFJc0kZGN4Mp1ecPCsjsnJwzCOSEph84a");

      /* set our custom set of headers */
      response = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);

      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);

      /* Perform the request, res will get the return code */
      response = curl_easy_perform(curl);
      /* Check for errors */
      if (response != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(response));
      else
        success = true;

      /* always cleanup */
      curl_easy_cleanup(curl);
    }

    return success;
  }


  bool Instance::sendPutRequest(const std::string& url, const std::string& body)
  {
    CURL *curl;
    CURLcode response;

    bool success = false;

    curl = curl_easy_init();
    if (curl) {
      struct curl_slist *chunk = NULL;

      chunk = curl_slist_append(chunk, "Accept: application/json");
      chunk = curl_slist_append(chunk, "X-Mashape-Key: iKMewtMiDYmshbuIQbFJc0kZGN4Mp1ecPCsjsnJwzCOSEph84a");

      /* set our custom set of headers */
      response = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      //curl_easy_setopt(curl, CURLOPT_PUT, 1L);
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */

      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str()); /* data goes here */

      /* Perform the request, res will get the return code */
      response = curl_easy_perform(curl);
      /* Check for errors */
      if (response != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(response));
      else
        success = true;

      /* always cleanup */
      curl_easy_cleanup(curl);
    }

    return success;
  }

}

