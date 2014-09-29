#include "Scene.hpp"

#include <iostream>
#include "MathCore.hpp"
#include "Printer.hpp"
#include "Renderer.hpp"
#include "OGLRenderEngine.hpp"
#include "Camera.hpp"
#include "Printer.hpp"
#include "Wumpus.hpp"
#include "jsoncpp.h"
#include "file_loader.h"

#include "LightNode.hpp"
#include "Cursor.hpp"
#include "UserInput.hpp"
#include "Treasure.hpp"
#include "Agent.hpp"
#include "Hud.hpp"
#include "Rotor.hpp"

using namespace matc;

Scene::Scene(int width, int height, UserInput *user):
  camera(Camera("SceneCamera", width, height)), user(user), width(width), height(height)
{
  root = new TransformNode("SceneRoot", Matrix4x4());

  camera.link(*root);
  light = new LightNode("Light 1", DirectionLight);
  light->ambientIntensity = 0.6f;
  light->diffuseIntensity = 0.8f;
  light->direction = Vector3(-1.0, -1.0, 1.0);
  root->addChild(light);

  cursor = new Cursor();
  hud = new Hud(this);
  //  cursor->link(*camera.getTransform());
  lastTime = glfwGetTime();

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
  delete agent;
  delete treasure;
}

bool Scene::load(std::string file)
{
  
  Json::Value rootValue;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(file_read(file.c_str()), rootValue);
  if(!parsingSuccessful) {
    std::cerr << "parsing error: " << reader.getFormattedErrorMessages() << std::endl;
  }

  std::string levelName = rootValue["LevelName"].asString();
  int width = rootValue["Width"].asInt();
  int height = rootValue["Height"].asInt();

  std::cout << "Read Level: " << levelName<< " with Width: " << width << ", Height: " << height << std::endl;
  Json::Value wumpusValue = rootValue["Wumpus"];
  Json::Value agentValue = rootValue["Agent"];
  Json::Value treasureValue = rootValue["Treasure"];
  /*
   * ==============
   * Agent, Wumpus, Treasure initialize here
   */
  agent = new Agent(agentValue["xpos"].asFloat(), agentValue["zpos"].asFloat());
  agent->link(*root);
  
  // Rotor *rotor = new Rotor("Rotor", 3.14f/100, Vector3(0, 1, 0), 4);
  
  wumpus = new Wumpus();
  wumpus->link(*root);
  wumpus->setPosition(wumpusValue["xpos"].asFloat(), wumpusValue["zpos"].asFloat());

  treasure = new Treasure(treasureValue["xpos"].asFloat(), treasureValue["zpos"].asFloat());
  treasure->link(*root);
  

  Json::Value pitsValue = rootValue["Pits"];
  std::vector<Json::Value> pitValues;
  for(int index = 0; index < pitsValue.size(); index++) {
    pitValues.push_back(pitsValue[index]);
  }
  
  this->level= new Level(width, height, pitValues);
  if(!this->level->linkLevel(*root)) {
    std::cout << "Couldn't link level to scene" << std::endl;
    return false;
  }
  
  return true;
}

void Scene::render(Renderer &renderer)
{
  root->accept(renderer);
  // maybe switch to agent vision here
  
  // update here?
  root->update(glfwGetTime());
}

void Scene::print(Printer &printer)
{
  root->accept(printer);
}

void Scene::switchMouseLook()
{
  if(camera.switchMouseLook()) {
    cursor->unlink(*camera.getTransform());
      user->setMousePosAction([&] (double xpos, double ypos) {
      camera.changeView(xpos, ypos, float(glfwGetTime()-lastTime));
    });
  } else {
    cursor->link(*camera.getTransform());
    user->setMousePosAction([&] (double xpos, double ypos) {
	double xdelta = xpos - (width/2);
	double ydelta = ypos - (height/2);
	cursor->translatePosition(xdelta, ydelta);
      });
  }
}

void Scene::resetCamera()
{
  camera.reset();
}
