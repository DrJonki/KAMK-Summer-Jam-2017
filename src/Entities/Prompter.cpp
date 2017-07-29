#include <Jam/Entities/Prompter.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Jam/Randomizer.hpp>

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

namespace jam
{
  Prompter::Prompter(Instance& ins)
    : Entity(),
      sf::Sprite(),
      m_indicator(),
      m_success(false),
      m_key(getRandomKey())
  {

  }

  void Prompter::setSuccess()
  {
    m_success = true;
  }

  bool Prompter::success() const
  {
    return m_success;
  }

  void Prompter::setPromptKey(const sf::Keyboard::Key key)
  {
    m_key = key;
  }

  sf::Keyboard::Key Prompter::promptKey() const
  {
    return m_key;
  }

  void Prompter::update(const float dt)
  {

  }

  void Prompter::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}
