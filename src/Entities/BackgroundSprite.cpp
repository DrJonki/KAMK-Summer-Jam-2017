#include <Jam/Entities/BackgroundSprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  BackgroundSprite::BackgroundSprite(Instance& ins, const std::string& texture)
    : Entity(),
      sf::RectangleShape(sf::Vector2f(
        ins.config.float_("VIEW_X"),
        ins.config.float_("VIEW_Y")
      ))
  {
    setTexture(&ins.resourceManager.GetTexture(texture));
  }

  void BackgroundSprite::update(const float dt)
  {
    
  }

  void BackgroundSprite::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}
