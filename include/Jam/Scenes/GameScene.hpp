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

    void textEvent(const uint32_t code) override;

  public:

    GameScene(Instance& ins);

    void update(const float dt) override;

    State getState() const;

    bool isStarted() const;

  private:

    void updateScores();

    State m_currentState;
    bool m_started;
    bool m_sendingScore;

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

    // Stats
    Text* m_scoreText;
    Text* m_startText;
    Text* m_endText;
    Text* m_leaderText;
    sf::String m_nameText;
    int m_score;
    int m_scoreExtra;

    void screenShake(const float dt);
    float m_screenShakeTimer = 0.f;
    bool m_isScreenShakeActive = false;
    sf::Vector2f m_shakePos;

  };
}