#ifndef SCENE_HPP
#define SCENE_HPP

#include "TransformNode.hpp"
#include "Camera.hpp"
#include "Level.hpp"

class Renderer;
class Printer;
class LightNode;
class Wumpus;
class Cursor;
class UserInput;
class Treasure;
class Hud;
class Agent;
class AgentLogic;

class Scene
{
public:
  Scene(int width, int height, UserInput *user);
  ~Scene();

  bool load(std::string file);
  void render(Renderer &renderer);
  void print(Printer &printer);
  Camera* getCamera() { return &camera; }
  void switchMouseLook();
  void resetCamera();

  void nextStep();

private:  
  // gets data from current field, maybe invoke getSensorData(x, y) from Scene/Level
  // maybe: adds safe fields to "border"
  void agentSense();

  // act appropriate to KI and Sensordata
  //   1. collects treasure if on field
  //   2. shoot wumpus if avaible
  //   3. goes to next safe field
  // order not sure
  void agentAct();

  
  int width, height;
  TransformNode *root;
  Camera camera;
  UserInput *user;
  Level *level;
  LightNode *light;

  // places of objects
  Wumpus *wumpus;
  Treasure *treasure;
  Agent *agent;
  //  std::vector<Pit*> pits;
  
  Cursor *cursor;

  Hud *hud;

  //test
  float lastTime;

  // !!!!!
  // maybe GameState class needed, practical?
  // saves position of all objects, and status of agent (sensor data)
  // !!!!!
  AgentLogic *ai;
  // Sensordata
  
};
#endif
