#pragma once

#include <Jam/Scene.hpp>

namespace jam
{
  class Player;
  class ParticleEmitter;

  class GameScene : public Scene
  {
  public:

    GameScene(Instance& ins);

    void update(const float dt) override;

  private:

    // Layers
    Layer& m_backgroundLayer;
    Layer& m_gameLayer;
    Layer& m_particleLayer;

    // Entities
    Player* m_player;
    ParticleEmitter* m_particleEmitter;
  };
}