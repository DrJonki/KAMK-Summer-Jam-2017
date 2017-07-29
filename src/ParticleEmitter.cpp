#include <Jam/ParticleEmitter.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  ParticleEmitter::ParticleEmitter(
    Instance& inst,
    std::string texturePath,
    sf::Vector2f textureSize,
    int maxParticles,
    float emitTime,
    float lifetime,
    float startSpeed,
    float friction,
    float startAngle,
    float startTorgue,
    float maxAlpha,
    float minAlpha
  ) : m_texture(inst.resourceManager.GetTexture(texturePath)),
    m_emitTime(emitTime),
    m_curTime(0.f),
    m_lifeTime(lifetime),
    m_startSpeed(startSpeed),
    m_friction(friction),
    m_startAngle(startAngle),
    m_startTorgue(startTorgue),
    m_maxAlpha(maxAlpha),
    m_minAlpha(minAlpha)
  {
    m_textureSize = textureSize;
    m_particles.reserve(maxParticles);
  }

  void ParticleEmitter::update(const float dt)
  {
    m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(), [dt](Particle& particle) {
      particle.update(dt);
      return particle.isDone;
    }), m_particles.end());

    m_curTime += dt;
    if (m_particles.size() < m_particles.capacity())
    {
      if (m_curTime < m_emitTime) {
        m_particles.push_back(Particle(
          m_texture,
          m_textureSize.x,
          m_textureSize.y,
          m_lifeTime,
          m_startSpeed,
          m_friction,
          m_startAngle,
          m_startTorgue,
          m_maxAlpha,
          m_minAlpha
        ));
      }
      else {
        if (m_particles.size() == 0)
          isDone = true;
      }
    }

    for (auto& particle : m_particles) {
      particle.update(dt);
    }
  }

  void ParticleEmitter::draw(sf::RenderTarget & target)
  {
    for (auto& particle : m_particles) {
      target.draw(particle);
    }
  }
}
