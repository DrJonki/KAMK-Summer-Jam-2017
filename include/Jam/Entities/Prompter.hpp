#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <Jam/Randomizer.hpp>
#include <Jam/ParticleEmitter.hpp>

namespace jam
{
  class Instance;

  class Prompter : public Entity, public sf::Sprite
  {
  public:

    Prompter(Instance& ins);

    void setSuccess();

    bool success() const;

    sf::Keyboard::Key promptKey() const;

  private:

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    sf::Keyboard::Key m_key;
    sf::Sprite m_indicator;
    sf::Sound m_clapSound;
    ParticleEmitter m_bottleParticle;
    Randomizer m_random;
    bool m_success;
    float m_timer;
  };
}
