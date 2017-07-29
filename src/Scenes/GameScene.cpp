#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/ParticleEmitter.hpp>
#include <Jam/Entities/Bottle.hpp>
#include <Jam/Entities/BackgroundSprite.hpp>
#include <iostream>
#include <iomanip>

namespace jam
{
  GameScene::GameScene(Instance& ins)
    : Scene(ins),

    // Layers
    m_backgroundLayer(addLayer(50)),
    m_gameLayer(addLayer(100)),
    m_particleLayer(addLayer(200)),

    // Entities
    m_player(nullptr)
  {
    const sf::Vector2f viewSize(sf::Vector2u(
      ins.config.integer("VIEW_X"),
      ins.config.integer("VIEW_Y")
    ));
    setView(sf::View(-viewSize * 0.5f, viewSize));
    m_backgroundLayer.setSharedView(&getView());
    m_gameLayer.setSharedView(&getView());
    m_particleLayer.setSharedView(&getView());

    // Background sprites
    // Sky
    //
    // � �
    // � �
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
    // � �  � �
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

    // Particle
    m_particleEmitter = &m_particleLayer.insert<ParticleEmitter>(
      "ParticleEmitter01",
      ins,
      "test.png",
      sf::Vector2f(50, 50),
      10000,
      0.25f
    );

    m_player = &m_gameLayer.insert<Player>("Player", ins);
    m_player->setOrigin(m_player->getSize().x * 0.5f, m_player->getSize().y);
    m_player->setPosition(0.f, viewSize.y - ins.config.float_("GROUND_LEVEL"));
  }

  void GameScene::update(const float dt)
  {
    Scene::update(dt);
    const auto& conf = getInstance().config;

    // Center camera on player
    auto view = getView();
    view.setCenter(
      m_player->getPosition().x,
      std::min(
        conf.float_("VIEW_Y") * 0.5f,
        m_player->getPosition().y
      )
    );
    setView(view);

    // Update background positions
    const auto all = m_backgroundLayer.getAll();
    for (auto& i : all) {
      i->setActive(false);
    }

    const int currentStageX = static_cast<unsigned int>((view.getCenter().x + (conf.float_("VIEW_X") * 0.5f)) / conf.float_("VIEW_X"));
    const int currentStageY = std::abs(static_cast<int>((view.getCenter().y - (conf.float_("VIEW_Y") * 0.5f)) / conf.float_("VIEW_Y")));
    const bool inSky = currentStageY > 0;
    std::cout << "Stage: [" << currentStageX << ", " << currentStageY << "]" << std::setw(100) << "\r";

    BackgroundSprite* bgs[] = {
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-tl")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-tr")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-bl")),
      static_cast<BackgroundSprite*>(m_backgroundLayer.get("bg-sky-br")),
    };

    if (!inSky) {
      const auto transitionAfter = conf.integer("GROUND_AMOUNT");
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
  }
}
