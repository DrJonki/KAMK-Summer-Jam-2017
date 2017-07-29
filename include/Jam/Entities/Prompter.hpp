#pragma once

#include <Jam/Entity.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace jam
{
  class Instance;

  class Prompter : public Entity, public sf::Sprite
  {
  public:

    Prompter(Instance& ins);

    void setSuccess();

    bool success() const;

    void setPromptKey(const sf::Keyboard::Key key);

    sf::Keyboard::Key promptKey() const;

  private:

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

    sf::Sprite m_indicator;
    bool m_success;
    sf::Keyboard::Key m_key;
  };
}
