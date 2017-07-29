#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/ParticleEmitter.hpp>
#include <Jam/Entities/Bottle.hpp>
#include <Jam/Entities/BackgroundSprite.hpp>

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
    setView(sf::View(
      sf::Vector2f(ins.window.getSize() / 2u),
      sf::Vector2f(ins.window.getSize())
    ));
    m_backgroundLayer.setSharedView(&getView());
    m_gameLayer.setSharedView(&getView());
    m_particleLayer.setSharedView(&getView());

    // Background sprites
    for (int i = -1; i < 10; ++i) {
      auto& spr = m_backgroundLayer.insert<BackgroundSprite>("Background-" + std::to_string(i), ins);
      spr.setPosition(static_cast<float>(i * static_cast<int>(ins.window.getSize().x)), 0.f);
    }

    // Player
    m_player = &m_gameLayer.insert<Player>("Player");
    m_player->setOrigin(0.f, m_player->getSize().y);
    m_player->setPosition(0.f, getView().getSize().y - m_player->getSize().y);

    // Particle
    m_particleEmitter = &m_particleLayer.insert<ParticleEmitter>(
      "ParticleEmitter01",
      ins,
      "test.png",
      sf::Vector2f(50, 50),
      100,
      9.f
    );
  }

  void GameScene::update(const float dt)
  {
    Scene::update(dt);

    auto view = getView();
    view.setCenter(m_player->getPosition().x, view.getCenter().y);
    setView(view);
  }
}
