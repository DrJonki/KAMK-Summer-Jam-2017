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
      m_backgroundLayer.insert<BackgroundSprite>("bg-sky-" + std::string(names[i]), ins);
    }
    // Ground
    //
    // ¤ ¤  ¤ ¤
    //
    for (std::size_t i = 0; i < 4; ++i) {
      const char* names[] = {
        "beach-l",
        "beack-r",
        "water-l",
        "water-r",
      };
      m_backgroundLayer.insert<BackgroundSprite>("bg-" + std::string(names[i]), ins);
    }

    // Player
    m_player = &m_gameLayer.insert<Player>("Player", ins);
    m_player->setOrigin(0.f, m_player->getSize().y);
    m_player->setPosition(0.f, ins.config.float_("GROUND_LEVEL"));
  }

  void GameScene::update(const float dt)
  {
    Scene::update(dt);

    // Center camera on player
    auto view = getView();
    view.setCenter(m_player->getPosition().x, view.getCenter().y);
    setView(view);

    enum {
      Ground,
      Middle,
      Water,
    };

    // Update background positions
    const auto currentStageX = static_cast<unsigned int>(m_player->getPosition().x / (getInstance().config.float_("VIEW_X")));
    // const auto currentStageY = 
  }
}
