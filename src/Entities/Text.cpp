#include <Jam/Entities/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace jam
{
  void Text::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}
