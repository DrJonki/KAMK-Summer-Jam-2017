#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Bottle.hpp>
#include <Jam/Entities/Prompter.hpp>
#include <Jam/ParticleEmitter.hpp>

namespace jam
{
  Player::Player(Instance& ins)
    : Entity(),
      AnimatedSprite(
        ins.resourceManager.GetTexture("Sprites/PlayerOne.png"),
        16,
        16,
        3,
        0.25f
      ),
      m_instance(ins),
      m_currentSpeed(
        ins.config.float_("PLAYER_START_SPEED"),
        0.f
      ),
      m_runParticle(
        ins, // instance
        "Particles/stepcloud.png", // texturePath
        sf::Vector2f(50, 50), // textureSize
        10, // maxParticles
        0.02f, // emitTime (if (x < 0.f) it is set to be forever
        0.50f, // lifeTime
        0.015f, // startspeed
        50.f, // friction
        0.f, // startAngle
        0.f, // startTorgue
        100.f, // maxAlpha
        0.f // minAlpha
      )
  {
    setScale(5.f, 5.f);
    m_jumpPressed = false;
    // Run particle
    m_runParticle.setPosition(getPosition());
  }

  void Player::update(const float dt)
  {
    using sf::Keyboard;

    AnimatedSprite::update(dt);
    m_runParticle.update(dt);

    static const auto gravity = m_instance.config.float_("GRAVITY");
    static const auto ground = m_instance.config.float_("GROUND_LEVEL");
    static const auto jumpForce = m_instance.config.float_("PLAYER_JUMP_FORCE");
    static const auto viewY = m_instance.config.float_("VIEW_Y");

    m_currentSpeed.y += gravity * dt;

    if (getPosition().y >= viewY - ground - 1.f) {
      m_currentSpeed.y = 0.f;
      // jump land effect
      if (m_jumpPressed)
      {
        m_runParticle.emit();
        m_jumpPressed = false;
      }
      // jump input
      if (Keyboard::isKeyPressed(Keyboard::Space))
      {
        m_currentSpeed.y = -jumpForce;
        m_jumpPressed = true;
      }
    }

    move(m_currentSpeed * dt);
    setPosition(getPosition().x, std::min(viewY - ground, getPosition().y));
    m_runParticle.setPosition(getPosition());
  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
    m_runParticle.draw(target);
  }

  bool Player::collide(Bottle& bottle)
  {
    if (bottle.getGlobalBounds().intersects(getGlobalBounds())) {
      bottle.setActive(false);

      return true;
    }

    return false;
  }

  bool Player::collide(Prompter& prompter)
  {
    if (!prompter.success() && prompter.getGlobalBounds().intersects(getGlobalBounds())) {
      if (sf::Keyboard::isKeyPressed(prompter.promptKey())) {
        static const auto accel = m_instance.config.float_("PLAYER_ACCELERATION");

        m_currentSpeed.x += accel;
        prompter.setSuccess();

        return true;
      }
    }

    return false;
  }
}