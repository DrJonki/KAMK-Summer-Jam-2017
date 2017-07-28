#include <Jam/AnimatedSprite.hpp>

namespace jam
{
    AnimatedSprite::AnimatedSprite(
        const std::string& fileName,
        int width,
        int height,
        int frames,
        float frameSpeed,
        int startFrame
    )
    {
        m_startFrame = startFrame;
        m_startSpeed = frameSpeed;
        reset();
    }

    void AnimatedSprite::update(const float dt)
    {
        m_curFrame = ;
        m_frameSpeed;
        m_curTime;
    }
    
    void AnimatedSprite::reset()
    {
        m_curTime = 0;
        m_curFrame = 0;
    }

    void AnimatedSprite::setSpeed(float speed)
    {
        m_frameSpeed = speed;
    }

    float AnimatedSprite::getSpeed()
    {
        return m_frameSpeed;
    }

    int AnimatedSprite::getWidth()
    {
        return m_frameWidth;
    }
       
    int AnimatedSprite::getHeight()
    {
        return m_frameHeight;
    }

}