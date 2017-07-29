#pragma once

#include <Jam/Entity.hpp>
#include <Jam/AnimatedSprite.hpp>

namespace jam
{
  class Instance;
  class Bottle final : public Entity, public AnimatedSprite
  {
  public:

    Bottle(Instance& inst);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  };
}