#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/Entities/Bottle.hpp>
#include <Jam/Entities/Prompter.hpp>
#include <Jam/Entities/Text.hpp>
#include <Jam/Entities/BackgroundSprite.hpp>
#include <SFML/Window/Joystick.hpp>
#include <Jam/Randomizer.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <string>

namespace jam
{
  GameScene::GameScene(Instance& ins)
    : Scene(ins),

    m_currentState(State::Running),
    m_started(false),
    m_sendingScore(false),

    // Sounds
    m_runMusic(),
    m_jumpMusic(),

    // Layers
    m_backgroundLayer(addLayer(50)),
    m_pickupLayer(addLayer(75)),
    m_gameLayer(addLayer(100)),
    m_particleLayer(addLayer(200)),
    m_uiLayer(addLayer(250)),

    // Entities
    m_player(nullptr),

    // Stats
    m_scoreText(nullptr),
    m_score(0),
    m_scoreExtra(0)
  {
    const sf::Vector2f viewSize(sf::Vector2u(
      ins.config.integer("VIEW_X"),
      ins.config.integer("VIEW_Y")
    ));
    setView(sf::View(-viewSize * 0.5f, viewSize));
    m_backgroundLayer.setSharedView(&getView());
    m_pickupLayer.setSharedView(&getView());
    m_gameLayer.setSharedView(&getView());
    m_particleLayer.setSharedView(&getView());
    m_uiLayer.setView(sf::View(viewSize * 0.5f, viewSize));

    const auto groundLevel = viewSize.y - ins.config.float_("GROUND_LEVEL");

    // Audio
    m_runMusic.openFromFile("assets/Audio/Music.wav");
    m_runMusic.setLoop(true);
    m_runMusic.setRelativeToListener(true);
    m_runMusic.setVolume(50.f);
    m_runMusic.play();

    m_jumpMusic.openFromFile("assets/Audio/FlyMusic.wav");
    m_jumpMusic.setRelativeToListener(true);
    m_jumpMusic.setVolume(75.f);

    m_buildUp.openFromFile("assets/Audio/Buildup.wav");
    m_buildUp.setRelativeToListener(true);
    m_buildUp.setPlayingOffset(sf::seconds(0.5f));

    // Background sprites
    // Sky
    //
    // ¤ ¤
    // ¤ ¤
    //
    for (std::size_t i = 0; i < 4; ++i) {
      const char* names[] = {
        "tl", // Top-Left
        "tr", // Top-Right
        "bl", // Bottom-Left
        "br", // Bottom-Right
      };
      const sf::Vector2f origins[] = {
        sf::Vector2f(1.f, 1.f),
        sf::Vector2f(0.f, 1.f),
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(1.f, 0.f),
      };
      auto& bg = m_backgroundLayer.insert<BackgroundSprite>("bg-sky-" + std::string(names[i]), ins, "BeachTop_BG_Pixelated.png");
      bg.setOrigin(sf::Vector2f(
        origins[i].x * bg.getSize().x,
        origins[i].y * bg.getSize().y
      ));
    }
    // Ground
    //
    // ¤ ¤  ¤ ¤
    //
    for (std::size_t i = 0; i < 4; ++i) {
      const char* names[] = {
        "beach-l",
        "beach-r",
        "water-l",
        "water-r",
      };
      const float origins[] = {
        1.f, 0.f,
      };
      auto& bg = m_backgroundLayer.insert<BackgroundSprite>("bg-" + std::string(names[i]), ins, i >= 2 ? "Water_BG_Pixelated.png" : "Beach_BG_Pixelated.png");
      bg.setOrigin(sf::Vector2f(origins[i % 2] * bg.getSize().x, 0.f));
    }

    const auto stageAmount = ins.config.integer("NUM_X_STAGES");
    
    // Prompters
    const auto prompterAmount = ins.config.integer("PROMPTER_AMOUNT");
    float lastPos = (viewSize.x / 2);
    for (int i = 1u; i < prompterAmount; ++i) {
      auto& prompter = m_pickupLayer.insert<Prompter>("Prompter", ins);
      Randomizer rand;
      const float xPos = lastPos + (viewSize.x / rand(1.f, 2.25f));
      prompter.setPosition(xPos, groundLevel + 10);
      lastPos = xPos;
    }

    // Bottles
    {
      float advance = 0.f;
      Randomizer rand;
      while (advance < (stageAmount - 1u) * viewSize.x) {
        auto& bottle = m_pickupLayer.insert<Bottle>("Bottle", ins);
        advance += rand(1, 6) * (viewSize.x / 6.f);
        bottle.setPosition(advance, viewSize.y - rand(1, 2) * ins.config.float_("GROUND_LEVEL") * 2.5f);
      }
    }

    m_player = &m_gameLayer.insert<Player>("Player", ins, *this);
    m_player->setOrigin(m_player->getLocalBounds().width * 0.5f, m_player->getLocalBounds().height);
    m_player->setPosition(0.f, groundLevel);

    auto view = getView();
    const auto current = glm::vec2(view.getCenter().x, view.getCenter().y);
    const auto target = glm::vec2(
      m_player->getPosition().x,
      std::min(
        ins.config.float_("VIEW_Y") * 0.5f,
        m_player->getPosition().y
      )
    );
    view.setCenter(target.x, target.y);
    setView(view);

    // Stats
    auto& font = ins.resourceManager.GetFont("gamefont.ttf");
    m_scoreText = &m_uiLayer.insert<Text>("ScoreText");
    m_scoreText->setFont(font);
    m_scoreText->setFillColor(sf::Color::Black);
    m_scoreText->setOutlineColor(sf::Color::White);
    m_scoreText->setOutlineThickness(2);
    m_scoreText->move(10, 0);

    m_startText = &m_uiLayer.insert<Text>("StartText");
    m_startText->setFont(font);
    m_startText->setFillColor(sf::Color::Black);
    m_startText->setOutlineColor(sf::Color::White);
    m_startText->setOutlineThickness(2);
    m_startText->setString(
      "ARROW KEYS to high five\n"
      "SPACE(A|B) to jump\n"
      "SPACE(A|B) to lock jump\n\n"
      "Press SPACE(ENTER) to begin"
    );
    m_startText->setCharacterSize(42);
    m_startText->setOrigin(m_startText->getLocalBounds().width * 0.5f, m_startText->getLocalBounds().height * 0.5f);
    m_startText->setPosition(viewSize * 0.5f);

    m_endText = &m_uiLayer.insert<Text>("EndText");
    m_endText->setFont(font);
    m_endText->setFillColor(sf::Color::Black);
    m_endText->setOutlineColor(sf::Color::White);
    m_endText->setOutlineThickness(2);
    m_endText->setPosition(10, 0);

    m_leaderText = &m_uiLayer.insert<Text>("NameText");
    m_leaderText->setFont(font);
    m_leaderText->setFillColor(sf::Color::Black);
    m_leaderText->setOutlineColor(sf::Color::White);
    m_leaderText->setCharacterSize(24);
    m_leaderText->setOutlineThickness(2);
    m_leaderText->setPosition(viewSize.x - 300, 0);
    m_leaderText->setActive(false);
    updateScores();
  }

  void GameScene::update(const float dt)
  {
    using sf::Keyboard;
    using sf::Joystick;

    Scene::update(dt);
    const auto& conf = getInstance().config;

    // Center camera on player
    auto view = getView();
    const auto current = glm::vec2(view.getCenter().x, view.getCenter().y);
    const auto target = glm::vec2(
      m_player->getPosition().x,
      std::min(
        conf.float_("VIEW_Y") * 0.5f,
        m_player->getPosition().y
      )
    );
    const auto result = glm::mix(
      glm::vec2(current),
      target,
      dt * 30.f
    );
    screenShake(dt);
    view.setCenter(result.x + m_shakePos.x, result.y + m_shakePos.y);
    setView(view);

    // Update background positions
    const auto all = m_backgroundLayer.getAll();
    for (auto& i : all) {
      i->setActive(false);
    }

    const int currentStageX = static_cast<unsigned int>((view.getCenter().x + (conf.float_("VIEW_X") * 0.5f)) / conf.float_("VIEW_X"));
    const int currentStageY = std::abs(static_cast<int>((view.getCenter().y - (conf.float_("VIEW_Y") * 0.5f)) / conf.float_("VIEW_Y")));
    const bool inSky = currentStageY > 0;
    //std::cout << "Stage: [" << currentStageX << ", " << currentStageY << "]" << std::setw(10) << "\r";

    BackgroundSprite* bgs[] = {
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-tl")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-tr")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-bl")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-br")),
    };

    const auto transitionAfter = conf.integer("NUM_X_STAGES");

    if (getState() == State::Jumped && !m_player->isStopped())
      m_scoreExtra = currentStageX - transitionAfter;

    if (!inSky) {
      char* left = "";
      char* right = "";
      // Beach -> water transition
      if (transitionAfter == currentStageX) {
        left = "bg-water-r";
        right = "bg-beach-l";
      }
      // Beach
      else if (transitionAfter > currentStageX) {
        left = "bg-beach-l";
        right = "bg-beach-r";
      }
      // Water
      else {
        left = "bg-water-l";
        right = "bg-water-r";
      }

      bgs[2] = static_cast<BackgroundSprite*>(m_backgroundLayer.get(left));
      bgs[3] = static_cast<BackgroundSprite*>(m_backgroundLayer.get(right));
    }

    for (int i = 0; i < 4; ++i) {
      bgs[i]->setPosition(
        conf.float_("VIEW_X") * currentStageX,
        -conf.float_("VIEW_Y") * currentStageY
      );
      bgs[i]->setActive(true);
    }

    if (getState() != State::Jumped && currentStageX >= transitionAfter && (view.getCenter().x + (conf.float_("VIEW_X") * 0.5f)) / conf.float_("VIEW_X") > (transitionAfter + 0.4f)) {
      m_currentState = State::BeforeJump;
      m_runMusic.stop();
      if (m_jumpMusic.getStatus() != sf::Music::Status::Playing) {
        m_jumpMusic.play();
        m_buildUp.play();
      }
      if (m_buildUp.getStatus() == sf::Music::Status::Stopped) {
        m_currentState = State::Jumped;
        if (!m_player->jump()) {
          m_jumpMusic.stop();
        }
      }
    }

    // Collisions
    for (auto& i : m_pickupLayer.getAll("Prompter")) {
      auto& prompter = *static_cast<Prompter*>(i);
      if (m_player->collide(prompter))
      {
        m_score += 10;
        m_isScreenShakeActive = true;
        m_runMusic.setPitch(1.f + m_player->getHandsSlapped()/20.f);
      }
      if (prompter.success() && getState() != State::Jumped)
      {
        prompter.setPlayerPos(m_player->getPosition());
      }
    }

    for (auto& i : m_pickupLayer.getAll("Bottle")) {
      m_score += m_player->collide(*static_cast<Bottle*>(i)) * 5;
    }

    // Stats
    m_scoreText->setString("Score: " + std::to_string(m_score + m_scoreExtra));

    if (Keyboard::isKeyPressed(Keyboard::Space) || Joystick::isButtonPressed(0, 9)) {
      m_startText->setActive(false);
      m_started = true;
    }

    m_endText->setActive(false);
    if (m_player->isStopped()) {
      m_scoreText->setActive(false);
      m_leaderText->setActive(true);

      const sf::Vector2f viewSize(sf::Vector2u(
        conf.integer("VIEW_X"),
        conf.integer("VIEW_Y")
      ));

      m_endText->setActive(true);
      m_endText->setString(
        "Final score: " + std::to_string(m_score + m_scoreExtra) +
        "\n\nPress R(SELECT) to jump again"
        "\n\nType your name and press\n"
        "ENTER to submit score\n> " + m_nameText);

      if (Keyboard::isKeyPressed(Keyboard::R) || Joystick::isButtonPressed(0, 8)) {
        getInstance().currentScene = std::make_unique<GameScene>(getInstance());
      }
    }
  }

  GameScene::State GameScene::getState() const
  {
    return m_currentState;
  }

  bool GameScene::isStarted() const
  {
    return m_started;
  }

  void GameScene::screenShake(const float dt)
  {
    if (m_isScreenShakeActive)
    {
      m_screenShakeTimer += dt;
      if (m_screenShakeTimer > 0.15f)
      {
        m_isScreenShakeActive = false;
        m_screenShakeTimer = 0.f;
        return;
      }
      Randomizer rand;
      m_shakePos = sf::Vector2f(
        rand(-10.2f, 10.2f),
        rand(-10.2f, 10.2f)
      );
    }
    else
    {
      m_shakePos = sf::Vector2f(0.f, 0.f);
    }
  }

  void GameScene::updateScores()
  {
    std::string str = "Highscores\n";
    auto& hs = getInstance().highscores;
    int num = 0;

    std::vector<std::pair<std::string, int>> vals(hs.begin(), hs.end());
    std::sort(vals.begin(), vals.end(), [](decltype(vals[0])& left, decltype(vals[0])& right) {
      return left.second > right.second;
    });

    if (vals.size() > 10) {
      vals.erase(vals.begin() + 9, vals.end());
    }

    for (auto itr = vals.begin(); itr != vals.end(); ++itr) {
      str += '\n';
      str += std::to_string(++num) + ". " + itr->first + " - " + std::to_string(itr->second);
    }

    m_leaderText->setString(str);
  }


  std::string url_encode(const std::string &value) {
    using namespace std;
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
      string::value_type c = (*i);

      // Keep alphanumeric and other accepted characters intact
      if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
        escaped << c;
        continue;
      }

      // Any other characters are percent-encoded
      escaped << uppercase;
      escaped << '%' << setw(2) << int((unsigned char)c);
      escaped << nouppercase;
    }

    return escaped.str();
  }

  void GameScene::textEvent(const uint32_t code)
  {
    if (m_player->isStopped() && !m_sendingScore) {

      if (code == 10 || code == 13) {
        m_sendingScore = true;
        auto& hs = getInstance().highscores;
        const auto finalScore = m_score + m_scoreExtra;
        const char* errText = "Score is lower\n> than existing!";

        if ((hs.find(m_nameText) == hs.end() && m_nameText != errText) || hs[m_nameText] < finalScore)
        {
          hs[m_nameText] = finalScore;

          if (getInstance().sendPutRequest("https://kvstore.p.mashape.com/collections/scores/items/" + url_encode(m_nameText), std::to_string(finalScore)))
            m_nameText = "Success!";
          else
            m_nameText = "Failed :(";

          getInstance().m_clock.restart();
        }
        else
        {
          m_sendingScore = false;
          m_nameText = errText;
          return;
        }

        if (hs.size() > 10) {
          int lowest = INT_MAX;
          auto lowItr = hs.end();

          for (auto itr = hs.begin(); itr != hs.end(); ++itr) {
            if (itr->second < lowest) {
              lowest = itr->second;
              lowItr = itr;
            }
          }

          hs.erase(lowItr);
        }

        updateScores();

        return;
      }

      sf::String current = m_nameText;

      if (code == '\b')
        current = current.substring(0, current.getSize() - 1);
      else {
        if (current.getSize() < 8)
          current += code;
      }

      m_nameText = current;
    }
  }
}
