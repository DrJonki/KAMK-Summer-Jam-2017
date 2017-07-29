#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/Entities/BackgroundSprite.hpp>

namespace jam
{
  GameScene::GameScene(Instance& ins)
    : Scene(ins),

      // Layers
      m_backgroundLayer(addLayer(50)),
      m_gameLayer(addLayer(100)),

      // Entities
      m_player(nullptr)
  {
    const sf::Vector2f viewSize(sf::Vector2u(
      ins.config.integer("VIEW_X"),
      ins.config.integer("VIEW_Y")
    ));
    setView(sf::View(viewSize * 0.5f, viewSize));
    m_backgroundLayer.setSharedView(&getView());
    m_gameLayer.setSharedView(&getView());

    // Background sprites
    for (int i = -1; i < 10; ++i) {
      auto& spr = m_backgroundLayer.insert<BackgroundSprite>("Background-" + std::to_string(i), ins);
      spr.setPosition(static_cast<float>(i * static_cast<int>(ins.window.getSize().x)), 0.f);


    }

    // Player
    m_player = &m_gameLayer.insert<Player>("Player", ins);
    m_player->setOrigin(0.f, m_player->getSize().y);
    m_player->setPosition(0.f, ins.config.float_("GROUND_LEVEL"));
  }

  void GameScene::update(const float dt)
  {
    Scene::update(dt);

    auto view = getView();
    view.setCenter(m_player->getPosition().x, view.getCenter().y);
    setView(view);
  }
}
