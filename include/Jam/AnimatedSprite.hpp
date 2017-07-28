#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics.hpp>

namespace jam
{
    class AnimatedSprite
    {
    public:

        AnimatedSprite(
            const std::string& fileName,
            int width,
            int height,
            int frames,
            float frameSpeed = 2.0f,
            int startFrame = 0
        );
        void update(const float dt);
        void reset();
        void setSpeed(float speed);
        float getSpeed();
        float getWidth();
        float getHeight();

    private:
        sf::Sprite
        int m_frameWidth;
        int m_frameHeight;
        int m_curFrame;
        int m_startFrame;
        float m_frameSpeed;
        float m_startSpeed;
        float m_curTime;
    };
}