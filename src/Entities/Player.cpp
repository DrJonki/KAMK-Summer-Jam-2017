#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  Player::Player(Instance& ins)
    : Entity(),
      sf::RectangleShape(sf::Vector2f(50.f, 50.f)),
      m_instance(ins),
      m_currentSpeed(
        ins.config.float_("PLAYER_START_SPEED"),
        0.f
      )
  {
    setFillColor(sf::Color::Red);
  }

  void Player::update(const float dt)
  {
    const auto gravity = m_instance.config.float_("GRAVITY");

    //m_currentSpeed.y -= 
    //m_currentSpeed

    move(m_currentSpeed * dt);
  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}