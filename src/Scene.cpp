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
#include "TextRenderer.hpp"

#include "LightNode.hpp"
#include "Cursor.hpp"
#include "UserInput.hpp"
#include "Treasure.hpp"
#include "Agent.hpp"
#include "Arrow.hpp"
#include "Hud.hpp"
#include "Hud_Text.hpp"
#include "Rotor.hpp"
#include "AgentLogic.hpp"
#include "LevelLogic.hpp"

using namespace matc;

Scene::Scene(int width, int height, UserInput *user):
  width(width), height(height), user(user), camera(Camera("SceneCamera", width, height)) //camera2(Camera("SceneCamera2", width, height))
{
  user->setWindowSizeAction([&] (int width, int height) {
      this->applyWindowSizeChange(Vector2i(width, height));
      });
  
  root = new TransformNode("SceneRoot", Matrix4x4());

  camera.link(*root);
  light = new LightNode("Light 1", DirectionLight);
  light->ambientIntensity = 0.3f;
  light->diffuseIntensity = 0.7f;
  light->direction = Vector3(0, -2.0, -1.0);
  root->addChild(light);

  cursor = new Cursor();
  hud = new Hud(this);
  hud_text = new Hud_Text(this);
  //cursor->link(*camera.getTransform());
  lastTime = glfwGetTime();

  hud->linkToCamera(*camera.getTransform());
  hud_text->linkToCamera(*camera.getTransform());

  // init text renderer
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

  arrow = new Arrow(0, 0);
  arrow->link(*agent->getPose());
  
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

  displayText.push_back("POINTS: ");
  displayText.push_back("");
  displayText.push_back("NEXT ACTION: ");
  displayText.push_back("");

  std::vector<Senses> senses = levelLogic->getSensorData();
  // input to agentlogic
  ai->inputNewSenses(std::set<Senses>(senses.begin(), senses.end()));
  // ask agentlogic for new action
  nextAction = ai->getNextAction();
  
  std::string actionSt = "";
  if(nextAction == Forward)
    actionSt = "FORWARD";
  if(nextAction == TurnLeft)
    actionSt = "TURNLEFT";
  if(nextAction == TurnRight)
    actionSt = "TURNRIGHT";
  if(nextAction == Grab)
    actionSt = "GRAB";
  if(nextAction == Shoot)
    actionSt = "SHOOT";
  if(nextAction == Leave)
    actionSt = "LEAVE";
  
  displayText[3] = actionSt;
  // ask levelLogic if action is possible, and update it if it's possible
  
  
  Vector2i agentPos = levelLogic->getAgentPos();
  agent->setPosition(gridToPos(agentPos.x), -gridToPos(agentPos.y));
  
  int points = levelLogic->getPoints();
  displayText[1] = std::to_string(points);

  return true;
}

void Scene::initTextRender()
{
  initText2D("../assets/ExportedFont.png", "textShader");
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

void Scene::applyWindowSizeChange(const Vector2i &newSize)
{
  this->width = newSize.x;
  this->height = newSize.y;
  camera.changeScreen(newSize);
}

void Scene::update(float deltaTime)
{
  // if(running) {
  camera.update(deltaTime);
  root->update(deltaTime);
  // }
  for(size_t i = 0; i < displayText.size(); i++) { 
    printText2D(displayText[i], TEXT_POS_X, TEXT_POS_Y - (1.5*TEXT_SIZE)*i, TEXT_SIZE);
  }
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
  x_diff = cursorPos.x - RESET_BUTTON_X;
  y_diff = cursorPos.y - RESET_BUTTON_Y;
  distance = sqrt(x_diff * x_diff + y_diff * y_diff);
  if(distance <= RESET_BUTTON_R) {
    resetScene();
  }
}

void Scene::nextStep()
{
  if(running) {
     bool actionWasSuccessful = levelLogic->isActionPossible(nextAction);
    if(nextAction == TurnLeft && actionWasSuccessful) {
      agent->rotate90Left();
    }
    else if(nextAction == TurnRight && actionWasSuccessful) {
      agent->rotate90Right();
    }
    else if(nextAction == Shoot && actionWasSuccessful) {
      wumpus->unlink(*worldTransform);
      animateArrowShot();
    }
    else if(nextAction == Grab && actionWasSuccessful) {
      treasure->unlink(*worldTransform);
    }
    else if(nextAction == Leave && actionWasSuccessful) {
      //running = false;
    }

    if(levelLogic->isLevelFinished()) {
      running = false;
      if(levelLogic->isAgentDead()) {
      } else {
        addTextToDisplay("GAME OVER");
        addTextToDisplay("PRESS RESET");
      }
    }

    // tell agent if action was successful
    ai->actionSucceeded(nextAction, actionWasSuccessful);
    // ask levellogic
    std::vector<Senses> senses = levelLogic->getSensorData();
    // input to agentlogic
    ai->inputNewSenses(std::set<Senses>(senses.begin(), senses.end()));
    // output agent experience
    outputAgentPercepts();
    // ask agentlogic for new action
    nextAction = ai->getNextAction();

    std::string actionSt = "";
    if(nextAction == Forward)
      actionSt = "FORWARD";
    if(nextAction == TurnLeft)
      actionSt = "TURNLEFT";
    if(nextAction == TurnRight)
      actionSt = "TURNRIGHT";
    if(nextAction == Grab)
      actionSt = "GRAB";
    if(nextAction == Shoot)
     actionSt = "SHOOT";
    if(nextAction == Leave)
      actionSt = "LEAVE";
    
    displayText[3] = actionSt;
    // ask levelLogic if action is possible, and update it if it's possible
   

    Vector2i agentPos = levelLogic->getAgentPos();
    agent->setPosition(gridToPos(agentPos.x), -gridToPos(agentPos.y));

    int points = levelLogic->getPoints();
    displayText[1] = std::to_string(points);
  }
}

void Scene::addTextToDisplay(std::string text)
{
  displayText.push_back(text);
}

void Scene::resetScene() {
  running = false;
  wumpus->unlink(*worldTransform);
  treasure->unlink(*worldTransform);
  agent->unlink(*worldTransform);
  displayText.clear();
  if(arrowShot) {
    arrow->unlink(*newArrowPos);
    worldTransform->removeChild(*newArrowPos);
    delete newArrowPos;
    arrowShot = false;
  } else {
    arrow->unlink(*agent->getPose());
  }
  delete arrow;
  delete wumpus;
  delete agent;
  delete treasure;
  load(levelFile);
  //resetCamera();
  running = true;
}

void Scene::animateArrowShot()
{
  arrowShot = true;
  if(arrow->unlink(*agent->getPose())) {
    newArrowPos = new TransformNode("ArrowShot-Pos",
						   translate(Matrix4x4(),
							     arrow->getPosition() + agent->getPosition()));
    worldTransform->addChild(newArrowPos);
    arrow->link(*newArrowPos);
    arrow->shoot();
  }
}

void Scene::outputAgentPercepts()
{
  auto save = ai->getSaveRooms();
  auto stenches = ai->getStenchPositions();
  auto breezes = ai->getBreezePositions();
  auto treasurePos = ai->getTreasurePos();

  std::cout << "Save Rooms:" << std::endl;
  for(auto it  = save.begin(); it != save.end(); it++) {
    Vector2i output = Vector2i(*it);
    std::cout << output << std::endl;
  }

  std::cout << "Stenches found:" << std::endl;
  for(auto it  = stenches.begin(); it != stenches.end(); it++) {
    Vector2i output = Vector2i(*it);
    std::cout << output << std::endl;
  }

  std::cout << "Breezes found:" << std::endl;
  for(auto it  = breezes.begin(); it != breezes.end(); it++) {
    Vector2i output = Vector2i(*it);
    std::cout << output << std::endl;
  }

  std::cout << "Treasure Room:" << std::endl;
  if(treasurePos != Vector2i(-1, -1)) {
    std::cout << treasurePos << std::endl;
  }
}
