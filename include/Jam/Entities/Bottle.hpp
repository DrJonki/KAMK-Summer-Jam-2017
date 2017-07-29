#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace jam
{
  class Instance;
  class Bottle final : public Entity, public sf::Sprite
  {
  public:

    Bottle(Instance& inst);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  private:

    float m_timer;
  };
}