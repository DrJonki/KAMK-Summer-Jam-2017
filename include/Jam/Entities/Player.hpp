#pragma once

#include <Jam/Entity.hpp>
#include <Jam/AnimatedSprite.hpp>
#include <SFML/Audio/Sound.hpp>
#include <Jam/ParticleEmitter.hpp>
#include <Jam/Randomizer.hpp>
#include <SFML/Audio.hpp>

namespace jam
{
  class Instance;
  class Bottle;
  class Prompter;
  class GameScene;

  class Player final : public Entity, public AnimatedSprite
  {
  public:

    Player(Instance& ins, GameScene& scene);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    bool jump();

    bool isStopped() const;

    bool collide(Bottle& bottle);

    bool collide(Prompter& prompter);

  private:
    Randomizer m_random;
    Instance& m_instance;
    GameScene& m_scene;
    sf::Vector2f m_currentSpeed;

    ParticleEmitter m_bottleParticle;
    ParticleEmitter m_runParticle;
    ParticleEmitter m_jumpParticle;

    float m_rotationSpeed;
    bool m_stopped;
    bool m_justJumped;
    bool m_jumpFailed;

    // Soundsource
    bool m_jumpPressed;
    sf::Sound m_bottleSound;
    sf::Sound m_runSound;
    sf::Sound m_finalJumpSound;
    sf::Sound m_ouchSound;
    sf::Sound m_splashSound;
    sf::Sound m_failSound;

    // arrow
    sf::Sprite m_arrowBar;
    sf::Sprite m_arrow;
    float m_arrow_angle = 0;
    int m_arrow_dir = 1;
    float m_arrow_angle_max = 1;
    float m_arrow_angle_min = -70;
    float m_arrow_speed = 800.f;
    bool m_arrow_locked = false;

  };
}