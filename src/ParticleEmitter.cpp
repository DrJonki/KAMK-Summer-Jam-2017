#include <Jam/ParticleEmitter.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  ParticleEmitter::ParticleEmitter(
    Instance& inst,
    std::string texturePath,
    sf::Vector2f textureSize,
    int maxParticles,
    float emitTime
  ) : m_texture(inst.resourceManager.GetTexture(texturePath))
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
      m_particles.push_back(Particle(m_texture, m_textureSize.x, m_textureSize.y));
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
