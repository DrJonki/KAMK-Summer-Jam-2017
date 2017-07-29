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

    void jump();

    bool collide(Bottle& bottle);

    bool collide(Prompter& prompter);

  private:
    Randomizer m_random;
    Instance& m_instance;
    GameScene& m_scene;
    sf::Vector2f m_currentSpeed;
    ParticleEmitter m_bottleParticle;
    ParticleEmitter m_runParticle;

    // Soundsource
    bool m_jumpPressed;
    sf::Sound m_bottleSound;
    sf::Sound m_runSound;
    sf::Sound m_chargeSound;
  };
}