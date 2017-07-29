#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/Text.hpp>

namespace jam
{
  class Text : public Entity, public sf::Text
  {
  public:

    using sf::Text::Text;


  private:

    void draw(sf::RenderTarget& target) override;
  };
}