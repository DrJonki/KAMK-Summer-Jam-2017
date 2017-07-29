#pragma once

#include <Jam/Scene.hpp>

namespace jam
{
  class Player;
  class BackgroundSprite;

  class GameScene : public Scene
  {
  private:

    enum {
      BG_TOPLEFT,
      BG_TOPRIGHT,
      BG_BOTTOMLEFT,
      BG_BOTTOMRIGHT,

      __BG_COUNT,
    };

  public:

    GameScene(Instance& ins);

    void update(const float dt) override;

  private:

    // Layers
    Layer& m_backgroundLayer;
    Layer& m_gameLayer;

    // Entities
    Player* m_player;
    BackgroundSprite* m_background[__BG_COUNT];
  };
}