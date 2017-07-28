#pragma once

#include <memory>
#include <vector>
#include <Jam/Entity.hpp>

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
    Entity& insert(Args&&... args) {
      static_assert(std::is_same<T, Entity>::value, "Not an entity");
      auto uniq = std::make_unique<T>(args...);
      auto& ref = *uniq;
      m_entities.emplace_back(std::move(uniq));

      return ref;
    }

    void update(const float dt);

    void draw(sf::RenderTarget& target);

  private:

    std::vector<std::unique_ptr<Entity>> m_entities;
  };
}

