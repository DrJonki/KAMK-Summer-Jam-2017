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

    void setPlayerPos(sf::Vector2f pos);

    sf::Keyboard::Key promptKey() const;

    int promptAxis() const;

    int promptValue() const;

  private:

    void update(const float dt) override;
    float lerp(float value, float start, float end);

    void draw(sf::RenderTarget& target) override;

    sf::Keyboard::Key m_key;
    sf::Sprite m_indicator;
    sf::Sound m_clapSound;
    ParticleEmitter m_bottleParticle;
    ParticleEmitter m_flyingBottle;
    Randomizer m_random;
    sf::Vector2f m_playerPos;
    float m_playerPosOffset;
    bool m_success;
    float m_timer;
  };
}
