#include <Jam/Entities/BackgroundSprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  BackgroundSprite::BackgroundSprite(Instance& ins)
    : Entity(),
      sf::RectangleShape(sf::Vector2f(ins.window.getSize()))
  {
    auto& tex = ins.resourceManager.GetTexture("Beach_BG_Pixelated.png");

    setTexture(&tex);
  }

  void BackgroundSprite::update(const float dt)
  {
    
  }

  void BackgroundSprite::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}
