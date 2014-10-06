#include "Scene.hpp"

#include <iostream>
#include "MathCore.hpp"
#include "Printer.hpp"
#include "Renderer.hpp"
#include "ShadowRenderer.hpp"
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
#include "Hud_Text.hpp"
#include "Rotor.hpp"
#include "AgentLogic.hpp"
#include "LevelLogic.hpp"

using namespace matc;

Scene::Scene(int width, int height, UserInput *user):
  width(width), height(height), user(user), camera(Camera("SceneCamera", width, height)) //camera2(Camera("SceneCamera2", width, height))
{
  root = new TransformNode("SceneRoot", Matrix4x4());

  camera.link(*root);
  light = new LightNode("Light 1", DirectionLight);
  light->ambientIntensity = 0.6f;
  light->diffuseIntensity = 0.7f;
  light->direction = Vector3(0, -3.0, -1.0);
  root->addChild(light);

  cursor = new Cursor();
  hud = new Hud(this);
  hud_text = new Hud_Text(this);
  //cursor->link(*camera.getTransform());
  lastTime = glfwGetTime();

  hud->linkToCamera(*camera.getTransform());
  hud_text->linkToCamera(*camera.getTransform());
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
  /*root->release();
  light->release();
  */
  delete wumpus;
  delete agent;
  delete treasure;
  delete cursor;
  delete hud;
}

bool Scene::load(const std::string &file)
{
  levelFile = std::string(file);
  Json::Value rootValue;
  Json::Reader reader;
  char* filecontent = file_read(file.c_str());
  if (filecontent == NULL) {
    //std::cerr << "Error: Couldn't load file: " << file << std::endl;
    return false;
  }
  bool parsingSuccessful = reader.parse(filecontent, rootValue);
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
  

  Json::Value pitsValue = rootValue["Pits"];
  std::vector<Json::Value> pitValues;
  for(unsigned int index = 0; index < pitsValue.size(); index++) {
    pitValues.push_back(pitsValue[index]);
  }
  
  this->level= new Level(width, height, pitValues);
  worldTransform = new TransformNode("WorldTransform", translate(Matrix4x4(), Vector3(-20, 5, 20)));
  root->addChild(worldTransform);
  if(!this->level->linkLevel(*worldTransform)) {
    std::cout << "Couldn't link level to scene" << std::endl;
    return false;
  }
   /*
   * ==============
   * Agent, Wumpus, Treasure initialize here
   */
  int agentX = agentValue["xpos"].asInt() - 1;
  int agentZ = agentValue["zpos"].asInt() - 1;
  agent = new Agent(gridToPos(agentX), -gridToPos(agentZ));
  agent->link(*worldTransform);
  
  // Rotor *rotor = new Rotor("Rotor", 3.14f/100, Vector3(0, 1, 0), 4);
  
  wumpus = new Wumpus();
  wumpus->link(*worldTransform);
  int wumpusX = (wumpusValue["xpos"].asInt()-1);
  int wumpusZ = (wumpusValue["zpos"].asInt()-1);

  wumpus->setPosition(gridToPos(wumpusX), -gridToPos(wumpusZ));

  int treasureX = treasureValue["xpos"].asInt() - 1;
  int treasureZ = treasureValue["zpos"].asInt() - 1;
  //std::cout << "grid: " << treasureX << ", " << treasureZ << std::endl;
  //std::cout << "world: " << gridToPos(treasureX) << ", " << -gridToPos(treasureZ) << std::endl;
  treasure = new Treasure(gridToPos(treasureX), -gridToPos(treasureZ));
  treasure->link(*worldTransform);

  ai = new AgentLogic();
  levelLogic = new LevelLogic(rootValue);
 
  return true;
}

void Scene::deleteScene()
{
  if(camera.mouseLookActive()) {
    // cursor root delete
  }
  root->release();
}

void Scene::render(Renderer &renderer)
{
  root->accept(renderer);
  // maybe switch to agent vision here
  
  // update here?
  //  root->update(glfwGetTime());
}

void Scene::render(ShadowRenderer &renderer)
{
  root->accept(renderer);
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

void Scene::switchCamera()
{
  camera.unlink(*root);
  //camera2.link(*root);
}

void Scene::update(float deltaTime)
{
  camera.update(deltaTime);
}

void Scene::clickCursor()
{
  Vector3 cursorPos = cursor->getPosition();
  float x_diff = cursorPos.x - NEXT_STEP_BUTTON_X;
  float y_diff = cursorPos.y - NEXT_STEP_BUTTON_Y;
  float distance = sqrt(x_diff * x_diff + y_diff * y_diff);
  if (distance <= NEXT_STEP_BUTTON_R) {
    //std::cout << "Next Step Button clicked" << std::endl;
    nextStep();
  }
}

void Scene::nextStep()
{
  if(running) {
    // ask levellogic
    std::vector<Senses> senses = levelLogic->getSensorData();
    // input to agentlogic
    ai->inputNewSenses(std::set<Senses>(senses.begin(), senses.end()));
    // ask agentlogic for new action
    Action nextAction = ai->getNextAction();
    
    std::cout << "next Action is: ";
    if(nextAction == Forward)
      std::cout << "Forward" << std::endl;
    if(nextAction == TurnLeft)
      std::cout << "TurnLeft" << std::endl;
    if(nextAction == TurnRight)
      std::cout << "TurnRight" << std::endl;
    if(nextAction == Grab)
      std::cout << "Grab" << std::endl;
    if(nextAction == Shoot)
      std::cout << "Shoot" << std::endl;
    if(nextAction == Leave)
      std::cout << "Leave" << std::endl;
    //nextAction = Forward;
    // ask levelLogic if action is possible, and update it if it's possible
    bool actionWasSuccessful = levelLogic->isActionPossible(nextAction);
    if(nextAction == Shoot && actionWasSuccessful) {
      wumpus->unlink(*worldTransform);
    }
    if(nextAction == Grab && actionWasSuccessful) {
      treasure->unlink(*worldTransform);
    }

    if(nextAction == Leave && actionWasSuccessful) {
      running = false;
    }

    if(levelLogic->isLevelFinished()) {
      running = false;
      if(levelLogic->isAgentDead()) {
      } else {
      }
    }

    // tell agent if action was successful
    ai->actionSucceeded(nextAction, actionWasSuccessful);

    //std::cout << "Action sucess: " << actionWasSuccessful << std::endl;
    /*
    // update levelLogic
    if (levelLogic->moveAgentTo(Vector2i(agentX, agentZ))) {
    // reposition model;
    agent->setPosition(gridToPos(agentX), -gridToPos(agentZ));
    }
    */
    Vector2i agentPos = levelLogic->getAgentPos();
    //  std::cout << "newAgentPos: " << agentPos << std::endl;
    agent->setPosition(gridToPos(agentPos.x), -gridToPos(agentPos.y));
  }
}

void Scene::resetScene() {
  running = true;
  load(levelFile);
  resetCamera();
}
