#include <Jam/Entities/Bottle.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
    Bottle::Bottle(Instance& inst)
      : Entity(),
      AnimatedSprite(
        inst.resourceManager.GetTexture("Sprites/Beer.png"),
        16,
        16,
        1,
        0.25f,
        0
      )
    {
        // we need something here?
    }


    void Bottle::update(const float dt)
    {
      AnimatedSprite::update(dt);
    }

    void Bottle::draw(sf::RenderTarget& target)
    {
      target.draw(*this);
    }
}