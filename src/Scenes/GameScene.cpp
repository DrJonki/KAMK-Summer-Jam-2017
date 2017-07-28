#include <Jam/Scenes/GameScene.hpp>
#include <Jam/Instance.hpp>

namespace jam
{
  GameScene::GameScene(Instance& ins)
    : Scene(ins)
  {
	  // sf::Texture testText = ins.resourceManager.GetTexture("test.png");
  }

  void GameScene::update(const float dt)
  {


    Scene::update(dt);
  }
}
