#include <Jam/Entities/Particle.hpp>

namespace jam {
  Particle::Particle(
    sf::Texture& texture,
    int width,
    int height,
    float lifetime,
    float startSpeed,
    float friction,
    float startAngle,
    float startTorgue,
    float maxAlpha,
    float minAlpha
  ) : m_width(width),
    m_height(height),
    m_startSpeed(startSpeed),
    m_friction(friction),
    m_curSpeed(startSpeed),
    m_curTime(0.f),
    m_lifeTime(lifetime),
	  m_torgue(startTorgue),
    m_maxAlpha(maxAlpha),
    m_minAlpha(minAlpha),
    m_random()
  {
    m_direction = sf::Vector2f(m_random(-1.f, 1.f), m_random(-1.f, 1.f));
    setTexture(texture);
    setRotation(startAngle);
    setOrigin(sf::Vector2f(m_width / 2, m_height / 2));
  }

  void Particle::update(const float dt)
  {
    m_curTime += dt;
    if (m_lifeTime >= m_curTime)
    {
      moveParticle(dt);
    }
    else
    {
      m_curTime = m_lifeTime;
      isDone = true;
    }
  }

  void Particle::moveParticle(const float dt)
  {
    auto moveVelocity = m_direction * (m_curSpeed - (m_friction * dt));
    move(moveVelocity);
    rotate(m_torgue);
    float what = 255.f - ((m_curTime / m_lifeTime) * 255.f);
    float alpha = glm::clamp(what, 0.f, 255.f);
    if (alpha <= 0)
      alpha = 0;
    setColor(sf::Color(255, 255, 255, alpha));
  }

}