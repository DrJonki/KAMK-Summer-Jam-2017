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

  void Layer::setView(const sf::View& view)
  {
    m_view = view;
  }

  sf::View Layer::getView() const
  {
    return m_view;
  }
}
