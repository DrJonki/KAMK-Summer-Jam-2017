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
      float emitTime,
      float lifetime = 1.f,
      float startSpeed = 2.f,
      float endSpeed = 0.f,
      float startAngle = 20.f,
      float startTorgue = 20.f
    );

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  private:

    std::vector<Particle> m_particles;
    sf::Texture& m_texture;
    sf::Vector2f m_textureSize;
    float m_emitTime;
    float m_curTime;
    float m_lifetime;
    float m_startSpeed;
    float m_endSpeed;
    float m_startAngle;
    float m_startTorgue;


  };
}