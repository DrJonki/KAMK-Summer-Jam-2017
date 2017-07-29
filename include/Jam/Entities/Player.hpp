#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace jam
{
  class Instance;
  class Bottle;
  class Prompter;

  class Player final : public Entity, public sf::RectangleShape
  {
  public:

    Player(Instance& ins);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    bool collide(Bottle& bottle);

    bool collide(Prompter& prompter);

  private:

    Instance& m_instance;
    sf::Vector2f m_currentSpeed;
  };
}