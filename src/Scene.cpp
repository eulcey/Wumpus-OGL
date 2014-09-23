#include "Scene.hpp"

#include <iostream>
#include "MathCore.hpp"
#include "Printer.hpp"
#include "Renderer.hpp"
#include "OGLRenderEngine.hpp"
#include "Camera.hpp"
#include "Printer.hpp"

using namespace matc;

Scene::Scene(int width, int height): camera(Camera("SceneCamera", width, height, 0))
{
  root = new TransformNode("SceneRoot", Matrix4x4());
  camera.link(*root);
}

bool Scene::load(std::string file)
{
  // this->world = // loadWorld(file);
  this->world.loadFile(file);
  //  std::cout << "world: " << this->world << std::endl;
  if(!this->world.linkWorld(*root)) {
    std::cout << "Couldn't link world to scene" << std::endl;
  }
}

void Scene::render(Renderer &renderer)
{
  root->accept(renderer);
}

void Scene::print(Printer &printer)
{
  root->accept(printer);
}
