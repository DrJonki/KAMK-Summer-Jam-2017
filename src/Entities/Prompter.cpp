#include <Jam/Entities/Prompter.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Jam/Instance.hpp>

sf::Keyboard::Key getRandomKey() {
  static const jam::Randomizer rand;

  const sf::Keyboard::Key keys[] = {
    sf::Keyboard::Left,
    sf::Keyboard::Right,
    sf::Keyboard::Up,
    sf::Keyboard::Down,
  };
  return keys[rand(0, 3)];
}

std::string getPromptTexture(const sf::Keyboard::Key key, const bool dude = false) {
  using Key = sf::Keyboard::Key;

  std::string prefix(dude ? "Sprites/Dude" : "Sprites/Arrow");

  switch (key)
  {
  case Key::Left:
    prefix += "Left.png";
    break;
  case Key::Right:
    prefix += "Right.png";
    break;
  case Key::Up:
    prefix += "Up.png";
    break;
  case Key::Down:
    prefix += "Down.png";
    break;
  default:
    assert(false);
  }

  return prefix;
}

namespace jam
{
  Prompter::Prompter(Instance& ins)
    : Entity(),
      sf::Sprite(),
      m_key(getRandomKey()),
      m_indicator(ins.resourceManager.GetTexture(getPromptTexture(m_key))),
      m_success(false),
      m_timer(0.f),
      m_clapSound(ins.resourceManager.GetSoundBuffer("HighFive.wav")),
      m_random(),
      m_bottleParticle(
        ins, // instance
        "Particles/highfiveeffect.png", // texturePath
        sf::Vector2f(25, 25), // textureSize
        30, // maxParticles
        0.45f, // emitTime (if (x < 0.f) it is set to be forever
        1.50f, // lifeTime
        2.75f, // startspeed
        0.5f, // friction
        0.f, // startAngle
        10.f, // startTorgue
        200.f, // maxAlpha
        0.f // minAlpha
      ),
      m_flyingBottle(
        ins, // instance
        "Particles/highfiveeffect.png", // texturePath
        sf::Vector2f(25, 25), // textureSize
        1, // maxParticles
        0.f, // emitTime (if (x < 0.f) it is set to be forever
        1.50f, // lifeTime
        2.75f, // startspeed
        0.5f, // friction
        0.f, // startAngle
        10.f, // startTorgue
        255.f, // maxAlpha
        255.f // minAlpha
      )
  {
    setTexture(ins.resourceManager.GetTexture(getPromptTexture(m_key, true)));
    setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height);
    setScale(2.5f, 2.5f);

    m_indicator.setOrigin(
      m_indicator.getLocalBounds().width / 2,
      m_indicator.getLocalBounds().height / 2
    );
    m_indicator.setScale(0.75f, 0.75f);
  }

  void Prompter::setSuccess()
  {
    m_bottleParticle.emit();
    m_flyingBottle.emit();
    float pitch = 1 + (m_random(-0.3f, 0.3f));
    m_playerPosOffset = m_random(-60, -10);
    m_clapSound.setPitch(pitch);
    m_clapSound.play();
    m_indicator.setScale(1.5f, 1.5f);
    m_success = true;
  }

  void Prompter::setPlayerPos(sf::Vector2f pos)
  {
    m_playerPos = pos;
  }

  bool Prompter::success() const
  {
    return m_success;
  }

  sf::Keyboard::Key Prompter::promptKey() const
  {
    return m_key;
  }

  void Prompter::update(const float dt)
  {
    if (!m_success)
    {
      const float floatRange = 6.f;
      const float floatSpeed = 5.f;
      const float floatOffset = -50.f;

      m_indicator.setPosition(
        m_indicator.getGlobalBounds().width / 2,
        floatOffset + std::sin((m_timer += dt) * floatSpeed) * floatRange
      );
    }
    else
    {
      const float rotAngle = 15.f;
      const float rotSpeed = 5.f;

      setRotation(std::sin((m_timer += dt) * rotSpeed) * rotAngle);

      const float endX = (m_playerPos.x + getLocalBounds().width + m_playerPosOffset);
      setPosition(
        lerp(dt*3, getPosition().x, endX),
        lerp(dt*3, getPosition().y, getPosition().y)
      );

      if (m_indicator.getScale().x > 0.f)
      {
        m_indicator.rotate(20.f);
        m_indicator.setScale(
          m_indicator.getScale().x - (dt * 4),
          m_indicator.getScale().y - (dt * 4)
        );
      }
      else
        m_indicator.setScale(0.f, 0.f);
    }
    m_bottleParticle.setPosition(sf::Vector2f(getPosition().x, getPosition().y - (getLocalBounds().height / 2)));
    m_bottleParticle.update(dt);
  }

  void Prompter::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
    m_bottleParticle.draw(target);
    sf::RenderStates states = sf::RenderStates::Default;
    states.transform = getTransform();
    target.draw(m_indicator, states);
  }

  float Prompter::lerp(float value, float start, float end)
  {
    return start + (end - start) * value;
  }

}
