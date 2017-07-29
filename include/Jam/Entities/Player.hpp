#pragma once

#include <Jam/Entity.hpp>
#include <Jam/AnimatedSprite.hpp>
#include <SFML/Audio/Sound.hpp>

namespace jam
{
  class Instance;
  class Bottle;
  class Prompter;
  class GameScene;

  class Player final : public Entity, public AnimatedSprite
  {
  public:

    Player(Instance& ins, GameScene& scene);

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    void jump();

    bool collide(Bottle& bottle);

    bool collide(Prompter& prompter);

  private:

    Instance& m_instance;
    GameScene& m_scene;
    sf::Vector2f m_currentSpeed;

    // Soundsource
    sf::Sound m_runSound;
    sf::Sound m_chargeSound;
  };
}