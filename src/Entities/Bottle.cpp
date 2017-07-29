#include <Jam/Entities/Bottle.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
    Bottle::Bottle(Instance& inst)
      : Entity(),
        sf::Sprite(inst.resourceManager.GetTexture("Sprites/Beer.png")),
        m_timer(0.f)
    {
      setScale(5.f, 5.f);

      const auto bounds = getLocalBounds();
      setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    }

    void Bottle::update(const float dt)
    {
      const float rotAngle = 30.f;
      const float rotSpeed = 5.f;

      setRotation(std::sin((m_timer += dt) * rotSpeed) * rotAngle);
    }

    void Bottle::draw(sf::RenderTarget& target)
    {
      target.draw(*this);
    }
}