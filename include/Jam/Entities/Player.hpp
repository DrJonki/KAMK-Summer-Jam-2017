#pragma once

#include <Jam/Entity.hpp>
#include <Jam/AnimatedSprite.hpp>
#include <Jam/ParticleEmitter.hpp>
#include <Jam/Randomizer.hpp>
#include <SFML/Audio.hpp>

namespace jam
{
  class Instance;
  class Bottle;
  class Prompter;

  class Player final : public Entity, public AnimatedSprite
  {
  public:

    Player(Instance& ins);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    bool collide(Bottle& bottle);

    bool collide(Prompter& prompter);

  private:
    Randomizer m_random;
    Instance& m_instance;
    sf::Vector2f m_currentSpeed;
    ParticleEmitter m_runParticle;
    bool m_jumpPressed;
    sf::Sound m_bottleSound;
  };
}