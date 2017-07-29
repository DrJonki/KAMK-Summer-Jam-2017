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
        ),
        m_timer(0.f)
    {
      setScale(5.f, 5.f);

      const auto bounds = getLocalBounds();
      setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    }

    void Bottle::update(const float dt)
    {
      AnimatedSprite::update(dt);

      const float rotAngle = 30.f;
      const float rotSpeed = 5.f;

      setRotation(std::sin((m_timer += dt) * rotSpeed) * rotAngle);
    }

    void Bottle::draw(sf::RenderTarget& target)
    {
      target.draw(*this);
    }
}