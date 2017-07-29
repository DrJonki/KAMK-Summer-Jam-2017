#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/Entities/Bottle.hpp>
#include <Jam/Entities/Prompter.hpp>
#include <Jam/Entities/Text.hpp>
#include <Jam/Entities/BackgroundSprite.hpp>
#include <Jam/Randomizer.hpp>
#include <iostream>
#include <iomanip>

namespace jam
{
  GameScene::GameScene(Instance& ins)
    : Scene(ins),

    m_currentState(State::Running),

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
    m_score(0)
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
    m_jumpMusic.setLoop(true);
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
    for (int i = 1u; i < stageAmount; ++i) {
      auto& prompter = m_pickupLayer.insert<Prompter>("Prompter", ins);
      prompter.setPosition(i * viewSize.x, groundLevel);
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

    // Stats
    m_scoreText = &m_uiLayer.insert<Text>("ScoreText");
    m_scoreText->setFont(ins.resourceManager.GetFont("gamefont.ttf"));
    m_scoreText->setFillColor(sf::Color::Black);
  }

  void GameScene::update(const float dt)
  {
    using sf::Keyboard;

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
      dt * 45.f
    );
    view.setCenter(result.x, result.y);
    setView(view);

    // Update background positions
    const auto all = m_backgroundLayer.getAll();
    for (auto& i : all) {
      i->setActive(false);
    }

    const int currentStageX = static_cast<unsigned int>((view.getCenter().x + (conf.float_("VIEW_X") * 0.5f)) / conf.float_("VIEW_X"));
    const int currentStageY = std::abs(static_cast<int>((view.getCenter().y - (conf.float_("VIEW_Y") * 0.5f)) / conf.float_("VIEW_Y")));
    const bool inSky = currentStageY > 0;
    std::cout << "Stage: [" << currentStageX << ", " << currentStageY << "]" << std::setw(10) << "\r";

    BackgroundSprite* bgs[] = {
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-tl")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-tr")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-bl")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-br")),
    };

    const auto transitionAfter = conf.integer("NUM_X_STAGES");

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
        m_player->jump();
      }
    }

    // Collisions
    for (auto& i : m_pickupLayer.getAll("Prompter")) {
      m_score += m_player->collide(*static_cast<Prompter*>(i)) * 10;
    }

    for (auto& i : m_pickupLayer.getAll("Bottle")) {
      m_score += m_player->collide(*static_cast<Bottle*>(i)) * 10;
    }

    // Stats
    m_scoreText->setString("Score: " + std::to_string(m_score));
  }

  GameScene::State GameScene::getState() const
  {
    return m_currentState;
  }
}
