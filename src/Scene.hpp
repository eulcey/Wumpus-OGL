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

const float NEXT_STEP_BUTTON_X = 1.94f;
const float NEXT_STEP_BUTTON_Y = -2.48f;
const float NEXT_STEP_BUTTON_R = 0.32f;

const float RESET_BUTTON_X = 2.97f;
const float RESET_BUTTON_Y = -1.54f;
const float RESET_BUTTON_R = 0.32f;

const int TEXT_SIZE = 18;
const int TEXT_POS_X = 12;
const int TEXT_POS_Y = 560;

class Scene
{
public:
  Scene(int width, int height, UserInput *user);
  ~Scene();

  void update(float deltaTime);

  bool load(const std::string &file);
  void initTextRender();
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
  bool mutexFree;
  
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


  void addTextToDisplay(std::string text);
  std::vector<std::string> displayText;
};
#endif
