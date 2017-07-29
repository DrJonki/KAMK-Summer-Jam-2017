#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace jam
{
  class Instance;

  class Player final : public Entity, public sf::RectangleShape
  {
  public:

    Player(Instance& ins);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  private:

    float m_currentSpeed;
    float m_jumpAccel;
  };
}