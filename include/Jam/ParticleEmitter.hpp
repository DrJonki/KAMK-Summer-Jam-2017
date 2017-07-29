#pragma once

#include <Jam/Entity.hpp>
#include <Jam/AnimatedSprite.hpp>
#include <Jam/Entities/Particle.hpp>
#include <vector>

namespace jam
{
  class Instance;
  class ParticleEmitter final : public Entity
  {
  public:

    ParticleEmitter(
      Instance& inst,
      std::string texturePath,
      sf::Vector2f textureSize,
      int maxParticles,
      float emitTime
    );

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  private:

    std::vector<Particle> m_particles;
    sf::Texture& m_texture;
    sf::Vector2f m_textureSize;
    float m_emitTime;
    float m_curTime;


  };
}