#include <Jam/Entities/Prompter.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Jam/Randomizer.hpp>
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

const char* getKeyTexture(const sf::Keyboard::Key key) {
  using Key = sf::Keyboard::Key;

  return "test.png";

  switch (key)
  {
  case Key::Left:
    return "";
  case Key::Right:
    return "";
  case Key::Up:
    return "";
  case Key::Down:
    return "";
  default:
    assert(false);
    return "";
  }
}

namespace jam
{
  Prompter::Prompter(Instance& ins)
    : Entity(),
      sf::Sprite(),
      m_key(getRandomKey()),
      m_indicator(ins.resourceManager.GetTexture(getKeyTexture(m_key))),
      m_success(false),
      m_timer(0.f)
  {
    const auto bounds = m_indicator.getGlobalBounds();
    m_indicator.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
  }

  void Prompter::setSuccess()
  {
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
    const float floatRange = 10.f;
    const float floatSpeed = 5.f;
    const float floatOffset = -100.f;

    m_indicator.setPosition(
      0.f,
      floatOffset + std::sin((m_timer += dt) * floatSpeed) * floatRange
    );
  }

  void Prompter::draw(sf::RenderTarget& target)
  {
    target.draw(*this);

    sf::RenderStates states = sf::RenderStates::Default;
    states.transform = getTransform();
    target.draw(m_indicator, states);
  }
}
