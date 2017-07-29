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
    float endSpeed,
    float startAngle,
    float startTorgue
  ) : m_texture(inst.resourceManager.GetTexture(texturePath)),
    m_emitTime(emitTime),
    m_curTime(0.f),
    m_lifetime(lifetime),
    m_startSpeed(startSpeed),
    m_endSpeed(endSpeed),
    m_startAngle(startAngle),
    m_startTorgue(startTorgue)
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

    if (m_particles.size() < m_particles.capacity())
    {
      m_particles.push_back(Particle(
        m_texture,
        m_textureSize.x,
        m_textureSize.y,
        m_emitTime,
        m_startSpeed,
        m_endSpeed,
        m_startAngle,
        m_startTorgue
      ));
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
