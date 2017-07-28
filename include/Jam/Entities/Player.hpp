#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace jam
{
  class Player final : public Entity, public sf::RectangleShape
  {
  public:

    Player();

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;
  };
}