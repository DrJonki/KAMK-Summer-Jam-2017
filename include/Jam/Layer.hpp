#pragma once

#include <memory>
#include <vector>
#include <Jam/Entity.hpp>
#include <SFML/Graphics/View.hpp>

namespace sf
{
  class RenderTarget;
}

namespace jam
{
  class Layer final
  {
  public:

    ~Layer();

    template<typename T, typename ... Args>
    T& insert(Args&&... args) {
      static_assert(std::is_base_of<Entity, T>::value, "Not an entity");
      auto uniq = std::make_unique<T>(args...);
      auto& ref = *uniq;
      m_entities.emplace_back(std::move(uniq));

      return ref;
    }

    void update(const float dt);

    void draw(sf::RenderTarget& target);

    void setView(const sf::View& view);

    sf::View getView() const;

  private:

    std::vector<std::unique_ptr<Entity>> m_entities;
    sf::View m_view;
  };
}

