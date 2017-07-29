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
    const float speed = 1000.f * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      move(-speed, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      move(speed, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
      move(0.f, -speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      move(0.f, speed);
  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}