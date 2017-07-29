#include <Jam/Entity.hpp>

namespace jam
{

  Entity::Entity()
    : m_active(true)
  {

  }

  Entity::~Entity()
  {}

  void Entity::baseUpdate(const float dt)
  {
    if (isActive()) {
      update(dt);
    }
  }

  void Entity::baseDraw(sf::RenderTarget& target)
  {
    if (isActive()) {
      draw(target);
    }
  }

  void Entity::update(const float dt)
  {}

  void Entity::draw(sf::RenderTarget& target)
  {}

  void Entity::setActive(const bool active)
  {
    m_active = active;
  }

  bool Entity::isActive() const
  {
    return m_active;
  }
}
