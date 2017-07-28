#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace jam
{
  class Instance;

  class BackgroundSprite : public Entity, public sf::RectangleShape
  {
  public:

    BackgroundSprite(Instance& ins);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  private:


  };
}
