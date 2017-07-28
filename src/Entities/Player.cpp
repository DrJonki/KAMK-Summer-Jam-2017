#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace jam
{
  Player::Player()
    : Entity(),
      sf::RectangleShape(sf::Vector2f(50.f, 50.f))
  {
    setFillColor(sf::Color::Red);
  }


  void Player::update(const float dt)
  {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      move(-10.f * dt, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      move(10.f * dt, 0.f);
  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}