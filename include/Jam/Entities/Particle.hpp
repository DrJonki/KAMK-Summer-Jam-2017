#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics.hpp>

namespace jam
{
  class Particle : public sf::Sprite
  {
  public:

    Particle(
      sf::Texture& texture,
      int width,
      int height,
      float lifetime = 1.f,
      float startSpeed = 2.f,
      float endSpeed = 0.f,
      float startAngle = 20.f,
      float startTorgue = 20.f
    );
    void update(const float dt);
    bool isDone = false;

  private:
    int m_width;
    int m_height;

    float m_startSpeed;
    float m_endSpeed;
    float m_curSpeed;

    float m_curTime;
    float m_lifeTime;

    void moveParticle(const float dt);
  };
}