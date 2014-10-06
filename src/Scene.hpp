#ifndef SCENE_HPP
#define SCENE_HPP

#include "TransformNode.hpp"
#include "Camera.hpp"
#include "Level.hpp"

class Renderer;
class ShadowRenderer;
class Printer;
class LightNode;
class Wumpus;
class Cursor;
class UserInput;
class Treasure;
class Hud;
class Hud_Text;
class Agent;
class AgentLogic;
class LevelLogic;

const float NEXT_STEP_BUTTON_X = 2.86f;
const float NEXT_STEP_BUTTON_Y = -2.43f;
const float NEXT_STEP_BUTTON_R = 0.36f;

class Scene
{
public:
  Scene(int width, int height, UserInput *user);
  ~Scene();

  void update(float deltaTime);

  bool load(const std::string &file);
  void deleteScene();
  void render(Renderer &renderer);
  void render(ShadowRenderer &renderer);
  void print(Printer &printer);
  Camera* getCamera() { return &camera; }
  void switchMouseLook();
  void resetCamera();
  void switchCamera();

  void clickCursor();
  void nextStep();

  void resetScene();

private:  
  // gets data from current field, maybe invoke getSensorData(x, y) from Scene/Level
  // maybe: adds safe fields to "border"
  //void agentSense();

  // act appropriate to KI and Sensordata
  //   1. collects treasure if on field
  //   2. shoot wumpus if avaible
  //   3. goes to next safe field
  // order not sure
  //void agentAct();

  
  int width, height;
  std::string levelFile;
  UserInput *user;
  Camera camera;
  //  Camera camera2;

  TransformNode *root = 0;
  Level *level = 0;
  LightNode *light = 0;
  TransformNode *worldTransform;
  // places of objects
  Wumpus *wumpus = 0;
  Treasure *treasure = 0;
  Agent *agent = 0;
  //  std::vector<Pit*> pits;
  
  Cursor *cursor = 0;

  Hud *hud = 0;
  Hud_Text *hud_text = 0;

  //test
  float lastTime;

  // !!!!!
  // maybe GameState class needed, practical?
  // saves position of all objects, and status of agent (sensor data)
  // !!!!!
  LevelLogic *levelLogic;
  AgentLogic *ai;

  bool running = true;
  // Sensordata
  
};
#endif
