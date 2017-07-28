#pragma once

#include <memory>
#include <map>

namespace sf
{
  class Drawable;
  class RenderTarget;
}

namespace jam
{
  class Layer;
  class Instance;

  class Scene
  {
  public:

    Scene(Instance& ins);

    virtual ~Scene() = 0;

    Layer& addLayer(const uint32_t weight);

    virtual void update(const float dt);

    virtual void draw(sf::RenderTarget& target);

    Instance& getInstance();

  private:

    Instance& m_instance;
    std::multimap<uint32_t, std::unique_ptr<Layer>> m_layers;
  };
}