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
        0.05f
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
        0.02f, // emitTime (if (x < 0.f) it is set to be forever
        1.f, // lifeTime
        0.05f, // startspeed
        0.025f, // friction
        0.f, // startAngle
        0.f, // startTorgue
        100.f, // maxAlpha
        0.f // minAlpha
      ),
      m_bottleParticle(
        ins,
        "Particles/beersplash.png",
        sf::Vector2f(25, 25),
        5,
        0.1f,
        0.50f,
        0.015f,
        0.1f,
        0.f,
        1.f,
        100.f,
        0.f
      ),
      m_bottleSound(ins.resourceManager.GetSoundBuffer("Yeah.wav"))
  {
    setScale(5.f, 5.f);

    m_runSound.setRelativeToListener(true);
    m_runSound.setLoop(true);
    m_jumpPressed = false;
    m_runSound.play();

    // Run particle
    m_runParticle.setPosition(getPosition());
    m_bottleParticle.setPosition(getPosition());
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

    switch (m_scene.getState()) {
    case GameScene::State::Running:
    {
      m_runSound.setVolume(100.f);

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
      break;
    }

    case GameScene::State::BeforeJump:
    {
      m_runSound.setVolume(0.f);

      break;
    }

    case GameScene::State::Jumped:
    {
      break;
    }
    }

    move(m_currentSpeed * dt);
    setPosition(getPosition().x, std::min(viewY - ground, getPosition().y));
    m_runParticle.setPosition(getPosition());
    m_bottleParticle.setPosition(
      sf::Vector2f(
        getPosition().x + getLocalBounds().width * 2,
        getPosition().y - getLocalBounds().height * 2
      )
    );
  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
    m_runParticle.draw(target);
    m_bottleParticle.draw(target);
  }

  bool Player::collide(Bottle& bottle)
  {
    if (bottle.getGlobalBounds().intersects(getGlobalBounds())) {
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

  void Player::jump()
  {
    static const float jumpX = m_instance.config.float_("PLAYER_FINAL_JUMP_X");
    static const float jumpY = m_instance.config.float_("PLAYER_FINAL_JUMP_Y");

    m_currentSpeed.x = jumpX;
    m_currentSpeed.y = jumpY;
  }
}