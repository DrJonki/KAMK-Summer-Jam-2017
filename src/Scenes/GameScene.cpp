#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Layer.hpp>
#include <Jam/Instance.hpp>
#include <Jam/Entities/Player.hpp>
#include <Jam/Entities/Bottle.hpp>

namespace jam
{
  GameScene::GameScene(Instance& ins)
    : Scene(ins),
      m_gameLayer(addLayer(100))
  {

    m_gameLayer.insert<Player>();
    m_gameLayer.insert<Bottle>(ins);
  }

  void GameScene::update(const float dt)
  {


    Scene::update(dt);
  }
}
