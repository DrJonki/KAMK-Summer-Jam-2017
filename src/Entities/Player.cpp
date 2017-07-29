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
    using sf::Keyboard;

    const auto gravity = m_instance.config.float_("GRAVITY");
    const auto ground = m_instance.config.float_("GROUND_LEVEL");
    const auto accel = m_instance.config.float_("PLAYER_ACCELERATION");
    const auto jumpForce = m_instance.config.float_("PLAYER_JUMP_FORCE");
    const auto viewY = m_instance.config.float_("VIEW_Y");

    m_currentSpeed.y += gravity * dt;

    if (getPosition().y >= viewY - ground - 1.f) {
      m_currentSpeed.y = 0.f;
      if (Keyboard::isKeyPressed(Keyboard::Space))
        m_currentSpeed.y = -jumpForce;
    }

    move(m_currentSpeed * dt);
    setPosition(getPosition().x, std::min(viewY - ground, getPosition().y));
  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
  }
}