#include <Jam/Entities/Bottle.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace jam
{
    Bottle::Bottle()
        : Entity(),
        sf::Sprite()
    {
        // we need something here?
    }


    void Bottle::update(const float dt)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            move(-10.f * dt, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            move(10.f * dt, 0.f);
    }

    void Bottle::draw(sf::RenderTarget& target)
    {
        target.draw(*this);
    }
}