#pragma once

namespace sf
{
  class RenderTarget;
}

namespace jam
{
  class Entity
  {
    friend class Layer;

  public:

    Entity();

    virtual ~Entity() = 0;

    void setActive(const bool active);

    bool isActive() const;

  private:

    void baseUpdate(const float dt);

    void baseDraw(sf::RenderTarget& target);

    virtual void update(const float dt);

    virtual void draw(sf::RenderTarget& target);

    bool m_active;
  };
}
