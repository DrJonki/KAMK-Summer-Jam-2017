#pragma once

#include <Jam/Scene.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Music.hpp>

namespace jam
{
  class Player;
  class ParticleEmitter;
  class BackgroundSprite;
  class Text;

  class GameScene : public Scene
  {
  public:

    enum class State {
      Running,
      BeforeJump,
      Jumped,
    };

  public:

    GameScene(Instance& ins);

    void update(const float dt) override;

    void setState(const State state);

    State getState() const;

  private:

    State m_currentState;

    // Sounds
    sf::Music m_runMusic;
    sf::Music m_jumpMusic;
    sf::Music m_buildUp;

    // Layers
    Layer& m_backgroundLayer;
    Layer& m_pickupLayer;
    Layer& m_gameLayer;
    Layer& m_particleLayer;
    Layer& m_uiLayer;

    // Entities
    Player* m_player;
    ParticleEmitter* m_particleEmitter;

    // Stats
    Text* m_scoreText;
    int m_score;
  };
}