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
        20, // maxParticles
        0.1f, // emitTime (if (x < 0.f) it is set to be forever
        1.50f, // lifeTime
        0.25f, // startspeed
        0.1f, // friction
        0.f, // startAngle
        10.f, // startTorgue
        200.f, // maxAlpha
        0.f // minAlpha
      )
  {
    setTexture(ins.resourceManager.GetTexture(getPromptTexture(m_key, true)));
    setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height);
    setScale(2.f, 2.f);

    m_indicator.setScale(0.75f, 0.75f);
  }

  void Prompter::setSuccess()
  {
    m_bottleParticle.emit();
    float pitch = 1 + (m_random(-0.3f, 0.3f));
    m_clapSound.setPitch(pitch);
    m_clapSound.play();
    m_success = true;
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
    const float floatRange = 6.f;
    const float floatSpeed = 5.f;
    const float floatOffset = -50.f;

    m_indicator.setPosition(
      0.f,
      floatOffset + std::sin((m_timer += dt) * floatSpeed) * floatRange
    );
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
}
