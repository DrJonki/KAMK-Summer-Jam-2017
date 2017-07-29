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
  ) 
  {
    setTexture(texture);
    m_width = width;
    m_height = height;

    m_startSpeed = startSpeed;
    m_endSpeed = endSpeed;
    m_curSpeed = startSpeed;

    m_curTime = 0.f;
    m_lifeTime = lifetime;
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
    // make it move here
  }

}