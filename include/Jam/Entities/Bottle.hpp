#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics.hpp>

namespace jam
{
    class Bottle final : public Entity, public sf::Sprite
    {
    public:

        Bottle();

        void update(const float dt) override;

        void draw(sf::RenderTarget& target) override;

    };
}