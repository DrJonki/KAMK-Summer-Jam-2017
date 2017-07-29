#include <Jam/Layer.hpp>
#include <Jam/Entity.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace jam
{
  Layer::Layer()
    : m_entities(),
      m_sharedView(nullptr),
      m_view()
  {

  }

  Layer::~Layer()
  {}

  void Layer::update(const float dt)
  {
    for (auto& i : m_entities) {
      i.second->update(dt);
    }
  }

  void Layer::draw(sf::RenderTarget& target)
  {
    for (auto& i : m_entities) {
      target.setView(m_sharedView ? *m_sharedView : m_view);
      i.second->draw(target);
    }
  }

  void Layer::setView(const sf::View& view)
  {
    m_view = view;
  }

  const sf::View& Layer::getView() const
  {
    return m_view;
  }

  void Layer::setSharedView(const sf::View* view)
  {
    m_sharedView = view;
  }

  const sf::View* Layer::getSharedView() const
  {
    return m_sharedView;
  }
}
