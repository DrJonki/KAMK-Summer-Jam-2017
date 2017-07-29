#include <Jam/Entities/Player.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Entities/Bottle.hpp>
#include <Jam/Entities/Prompter.hpp>

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
      m_scene(scene),
      m_currentSpeed(
        ins.config.float_("PLAYER_START_SPEED"),
        0.f
      ),
      m_runSound(ins.resourceManager.GetSoundBuffer("Run.wav"))
  {
    setScale(5.f, 5.f);

    m_runSound.setRelativeToListener(true);
    m_runSound.setLoop(true);
    m_runSound.play();
  }

  void Player::update(const float dt)
  {
    using sf::Keyboard;

    AnimatedSprite::update(dt);

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
        if (Keyboard::isKeyPressed(Keyboard::Space))
          m_currentSpeed.y = -jumpForce;
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
  }

  void Player::draw(sf::RenderTarget& target)
  {
    target.draw(*this);
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

  void Player::jump()
  {
    static const float jumpX = m_instance.config.float_("PLAYER_FINAL_JUMP_X");
    static const float jumpY = m_instance.config.float_("PLAYER_FINAL_JUMP_Y");

    m_currentSpeed.x = jumpX;
    m_currentSpeed.y = jumpY;
  }
}