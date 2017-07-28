#pragma once

namespace sf
{
  class RenderTarget;
}

namespace jam
{
  class Entity
  {
  public:

    virtual ~Entity() = 0;

    virtual void update(const float dt);

    virtual void draw(sf::RenderTarget& target);
  };
}
