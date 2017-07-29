#include <Jam/Entities/Particle.hpp>

namespace jam {
  Particle::Particle(
    sf::Texture& texture,
    int width,
    int height,
    float lifetime,
    float startSpeed,
    float endSpeed,
    float startAngle,
    float startTorgue
  ) : m_width(width),
    m_height(height),
    m_startSpeed(startSpeed),
    m_endSpeed(endSpeed),
    m_curSpeed(startSpeed),
    m_curTime(0.f),
    m_lifeTime(lifetime)
  {
    setTexture(texture);
  }

  void Particle::update(const float dt)
  {
    m_curTime += dt;
    if (m_lifeTime > m_curTime)
    {
      moveParticle(dt);
    }
    else
    {
      isDone = true;
    }
  }

  void Particle::moveParticle(const float dt)
  {
    auto direction = sf::Vector2f(1, -1);
    auto moveVelocity = direction * m_curSpeed;
    move(moveVelocity);
    rotate(1.f);
  }

}