#include <Jam/Layer.hpp>
#include <Jam/Entity.hpp>

namespace jam
{
  Layer::~Layer()
  {}

  void Layer::update(const float dt)
  {
    for (auto& i : m_entities) {
      i->update(dt);
    }
  }

  void Layer::draw(sf::RenderTarget& target)
  {
    for (auto& i : m_entities) {
      i->draw(target);
    }
  }
}
