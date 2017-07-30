#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Entities/Bottle.hpp>
#include <Jam/Entities/Prompter.hpp>
#include <Jam/ParticleEmitter.hpp>

namespace jam
{
  Player::Player(Instance& ins, GameScene& scene)
    : Entity(),
      AnimatedSprite(
        ins.resourceManager.GetTexture("Sprites/PlayerOne.png"),
        16,
        16,
        3,
        0.15f
      ),
      m_instance(ins),
      m_random(),
      m_scene(scene),
      m_currentSpeed(
        ins.config.float_("PLAYER_START_SPEED"),
        0.f
      ),
      m_runSound(ins.resourceManager.GetSoundBuffer("Run.wav")),
      m_runParticle(
        ins, // instance
        "Particles/stepcloud.png", // texturePath
        sf::Vector2f(50, 50), // textureSize
        3, // maxParticles
        0.25f, // emitTime (if (x < 0.f) it is set to be forever
        1.f, // lifeTime
        0.55f, // startspeed
        0.25f, // friction
        0.f, // startAngle
        0.f, // startTorgue
        100.f, // maxAlpha
        0.f // minAlpha
      ),
      m_bottleParticle(
        ins,
        "Particles/beersplash.png",
        sf::Vector2f(25, 25),
        15,
        0.35f,
        0.50f,
        1.55f,
        0.5f,
        0.f,
        1.75f,
        100.f,
        0.f
      ),
      m_jumpParticle(
        ins,
        "Particles/highfiveeffect.png",
        sf::Vector2f(25, 25),
        200,
        8.f,
        1.50f,
        2.75f,
        0.5f,
        0.f,
        10.f,
        200.f,
        0.f
      ),
      m_slaps(0),
      m_bottleSound(ins.resourceManager.GetSoundBuffer("bottleSound.wav")),
      m_finalJumpSound(ins.resourceManager.GetSoundBuffer("FinalJump.wav")),
      m_arrow(ins.resourceManager.GetTexture("arrow.png")),
      m_arrowBar(ins.resourceManager.GetTexture("arrowBar.png")),
      m_stopped(false),
      m_jumpPressed(false),
      m_rotationSpeed(0.f),
      m_ouchSound(ins.resourceManager.GetSoundBuffer("auts2_louder.wav")),
      m_splashSound(ins.resourceManager.GetSoundBuffer("Splash.wav")),
      m_failSound(ins.resourceManager.GetSoundBuffer("Fart.wav")),
      m_jumpFailed(false)
  {
    setScale(5.f, 5.f);

    m_runSound.setRelativeToListener(true);
    m_runSound.setLoop(true);
    m_runSound.play();

    m_bottleSound.setRelativeToListener(true);
    m_finalJumpSound.setRelativeToListener(true);
    m_bottleSound.setRelativeToListener(true);
    m_ouchSound.setRelativeToListener(true);
    m_splashSound.setRelativeToListener(true);
    m_splashSound.setPitch(1.2f);
    m_failSound.setRelativeToListener(true);
    m_failSound.setPitch(0.8f);

    m_arrow.setOrigin(0, m_arrow.getLocalBounds().height / 2);
    m_arrowBar.setOrigin(0, m_arrowBar.getLocalBounds().height / 2);
    m_arrow.setPosition(0, -1000);
    m_arrowBar.setPosition(0, -1000);
    m_arrowBar.setColor(sf::Color(255, 255, 255, 75));
    m_arrowBar.setColor(sf::Color(255, 255, 255, 100));
  }

  int Player::getHandsSlapped()
  {
    return m_slaps;
  }

  void Player::update(const float dt)
  {
    using sf::Keyboard;

    AnimatedSprite::update(dt);
    m_runParticle.update(dt);
    m_bottleParticle.update(dt);

    static const auto gravity = m_instance.config.float_("GRAVITY");
    static const auto ground = m_instance.config.float_("GROUND_LEVEL");
    static const auto jumpForce = m_instance.config.float_("PLAYER_JUMP_FORCE");
    static const auto viewY = m_instance.config.float_("VIEW_Y");

    if (!m_scene.isStarted())
      m_runSound.setVolume(0.f);

    if (!m_stopped && m_scene.isStarted()) {
      switch (m_scene.getState()) {
      case GameScene::State::Running:
      {
        m_runSound.setVolume(100.f);
        AnimatedSprite::setSpeed(0.05f + (0.10f / std::max(1, m_slaps)));
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
          if (Keyboard::isKeyPressed(Keyboard::Space) || sf::Joystick::isButtonPressed(0, 1) || sf::Joystick::isButtonPressed(0, 2))
          {
              m_currentSpeed.y = -jumpForce;
              m_jumpPressed = true;
          }
        }

        m_arrow_speed = 1650.f - m_currentSpeed.x;
        break;
      }

      case GameScene::State::BeforeJump:
      {
        m_runSound.setVolume(0.f);
        m_arrow.setPosition(
          getPosition().x + getLocalBounds().width * 2,
          getPosition().y - getLocalBounds().height * 3
        );
        m_arrowBar.setPosition(
          getPosition().x + getLocalBounds().width * 2.5f,
          getPosition().y - getLocalBounds().height * 6.f
        );
        m_currentSpeed = sf::Vector2f();

        if (Keyboard::isKeyPressed(Keyboard::Space) || sf::Joystick::isButtonPressed(0, 1) || sf::Joystick::isButtonPressed(0, 2))
        {
          m_arrow_locked = true;
        }
        if (!m_arrow_locked)
        {
          // rotate
          m_arrow_angle += m_arrow_speed * dt * m_arrow_dir;

          m_arrow.setRotation(m_arrow_angle);
          // change dir
          if (m_arrow_angle > m_arrow_angle_max)
          {
            m_arrow.setRotation(m_arrow_angle_max - 0.15f);
            m_arrow_angle = m_arrow_angle_max - 0.15f;
            m_arrow_dir = -1;
          }
          else if (m_arrow_angle < m_arrow_angle_min)
          {
            m_arrow.setRotation(m_arrow_angle_min + 0.15f);
            m_arrow_angle = m_arrow_angle_min + 0.15f;
            m_arrow_dir = 1;
          }
        }

        break;
      }

      case GameScene::State::Jumped:
      {
        if (!m_justJumped) 
        {
          m_currentSpeed.y += gravity * dt;
          m_currentSpeed.x = std::max(0.f, m_currentSpeed.x);

          if (getPosition().y >= viewY - ground - 1.f) 
          {
            m_currentSpeed.y *= -0.65f;
            m_currentSpeed.x *= 0.65f;
            m_rotationSpeed += 90.f;
            m_ouchSound.setPitch(m_random.range(0.7f, 1.5f));
            m_ouchSound.play();
          }

          if (m_currentSpeed.y < 50.f && m_currentSpeed.x < 50.f) 
          {
            m_stopped = true;
            m_splashSound.play();
          }
        }
        m_justJumped = false;
        m_jumpParticle.setPosition(getPosition());
        m_jumpParticle.setStartRotation(m_random(0.f, 360.f));
        m_jumpParticle.update(dt);
      }
      }
    }

    if (m_scene.isStarted()) {
      rotate(m_rotationSpeed * dt);
      move(m_currentSpeed * dt);
      setPosition(getPosition().x, std::min(viewY - ground, getPosition().y));
      // set all particles to follow player
      m_runParticle.setPosition(getPosition());
      m_bottleParticle.setPosition(
        sf::Vector2f(
          getPosition().x + getLocalBounds().width * 2,
          getPosition().y - getLocalBounds().height * 2
        )
      );
    }
  }

  void Player::draw(sf::RenderTarget& target)
  {
    if (!isStopped()) {
      target.draw(*this);
      target.draw(m_arrowBar);
      target.draw(m_arrow);
      m_runParticle.draw(target);
      m_bottleParticle.draw(target);
      m_jumpParticle.draw(target);
    }
  }

  sf::FloatRect& modifyRect(sf::FloatRect& rect) {
    rect.width *= 0.5f;
    rect.height *= 0.5f;
    return rect;
  }

  bool Player::collide(Bottle& bottle)
  {
    if (modifyRect(bottle.getGlobalBounds()).intersects(modifyRect(getGlobalBounds()))) {
      bottle.setActive(false);
      m_bottleParticle.emit();
      float pitch = 1 + (m_random(-0.3f, 0.3f));
      m_bottleSound.setPitch(pitch);
      m_bottleSound.play();
      return true;
    }

    return false;
  }

  bool Player::collide(Prompter& prompter)
  {
    if (!prompter.success() && modifyRect(prompter.getGlobalBounds()).intersects(modifyRect(getGlobalBounds()))) {
      if (sf::Keyboard::isKeyPressed(prompter.promptKey()) || sf::Joystick::getAxisPosition(0, static_cast<sf::Joystick::Axis>(prompter.promptAxis())) == prompter.promptValue()) {
        static const auto accel = m_instance.config.float_("PLAYER_ACCELERATION");
        m_slaps += 1;
        m_currentSpeed.x += accel;
        prompter.setSuccess();

        return true;
      }
    }

    return false;
  }

  bool Player::jump()
  {
    static const float jumpX = m_instance.config.float_("PLAYER_FINAL_JUMP_X");
    static const float jumpY = m_instance.config.float_("PLAYER_FINAL_JUMP_Y");
    static const float slapAmount = m_instance.config.float_("PROMPTER_AMOUNT");

    const float range = std::abs(m_arrow_angle_min - m_arrow_angle_max);
    const float ratio = 1.f - std::abs(std::abs(m_arrow_angle) - range * 0.5f) / (range * 0.5f);
    const float arrowFail = !m_arrow_locked || ratio < 0.1f;

    m_currentSpeed.x = jumpX;
    m_currentSpeed.y = -jumpY;
    m_justJumped = true;
    m_rotationSpeed = 180.f;

    if (arrowFail) {
      m_failSound.play();
      m_currentSpeed *= 0.075f;
      m_jumpFailed = true;
    }
    else
    {
      m_jumpParticle.emit();
      m_currentSpeed *= std::min((ratio * (std::max(m_slaps, 1) / slapAmount)) * 1.25f, 1.f);
      m_finalJumpSound.play();
    }

    return !arrowFail;
  }

  bool Player::isStopped() const
  {
    return m_stopped;
  }

}