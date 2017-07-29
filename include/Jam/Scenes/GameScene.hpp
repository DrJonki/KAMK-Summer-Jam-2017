#pragma once

#include <Jam/Scene.hpp>

namespace jam
{
  class Player;

  class GameScene : public Scene
  {
  public:

    GameScene(Instance& ins);

    void update(const float dt) override;

  private:

    // Layers
    Layer& m_backgroundLayer;
    Layer& m_gameLayer;

    // Entities
    Player* m_player;
  };
}