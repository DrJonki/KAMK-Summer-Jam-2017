#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  Player::Player(Instance& ins)
    : Entity(),
      sf::RectangleShape(sf::Vector2f(50.f, 50.f)),
      m_currentSpeed(250.f),
      m_jumpAccel(0.f)
  {
    setFillColor(sf::Color::Red);
  }

  void Player::update(const float dt)
  {
    /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      move(-500.f * dt, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      move(500.f * dt, 0.f);*/


    move(m_currentSpeed * dt, 0.f);

  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}