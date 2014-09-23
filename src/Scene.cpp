#include "Scene.hpp"

#include <iostream>
#include "MathCore.hpp"
#include "Printer.hpp"
#include "Renderer.hpp"
#include "OGLRenderEngine.hpp"
#include "Camera.hpp"
#include "Printer.hpp"
#include "Wumpus.hpp"

#include "LightNode.hpp"

using namespace matc;

Scene::Scene(int width, int height): camera(Camera("SceneCamera", width, height))
{
  root = new TransformNode("SceneRoot", Matrix4x4());
  camera.link(*root);
  light = new LightNode("Light 1", DirectionLight);
  light->ambientIntensity = 0.6f;
  light->diffuseIntensity = 0.8f;
  light->direction = Vector3(-1.0, -1.0, 1.0);
  root->addChild(light);
  /*
  ModelNode skybox_sky("Skybox_sky", "../assets/skybox_sky.obj");
  MaterialNode skybox_sky_material("Skybox Sky Material", "Skybox_sky", "texturedShader");
  skybox_sky_material.addChild(&skybox_sky);

  TransformNode skyboxScale("Skybox Scale", scale(Matrix4x4(), 10.0f, 10.0f, 10.0f));
  skyboxScale.addChild(&skybox_sky_material);
  camera.addSkybox(skyboxScale);
  */
}

Scene::~Scene()
{
  root->release();
  light->release();
  delete wumpus;
}

bool Scene::load(std::string file)
{
  // this->world = // loadWorld(file);
  this->world.loadFile(file);
  //  std::cout << "world: " << this->world << std::endl;
  if(!this->world.linkWorld(*root)) {
    std::cout << "Couldn't link world to scene" << std::endl;
    return false;
  }
  wumpus = new Wumpus();
  wumpus->link(*root);
  wumpus->setPosition(0.0f, 0.0f);
  
  return true;
}

void Scene::render(Renderer &renderer)
{
  root->accept(renderer);
}

void Scene::print(Printer &printer)
{
  root->accept(printer);
}
