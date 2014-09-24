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
private:
  int width, height;
  TransformNode *root;
  Camera camera;
  UserInput *user;
  Level *level;
  LightNode *light;

  Wumpus *wumpus;
  Treasure *treasure;
  Cursor *cursor;

  Hud *hud;

  //test
  float lastTime;
};
#endif
