#ifndef SCENE_HPP
#define SCENE_HPP

#include "TransformNode.hpp"
#include "Camera.hpp"
#include "World.hpp"

class Renderer;
class Printer;
class LightNode;
class Wumpus;

class Scene
{
public:
  Scene(int width, int height);
  ~Scene();

  bool load(std::string file);
  void render(Renderer &renderer);
  void print(Printer &printer);
  Camera* getCamera() { return &camera; }
private:
  TransformNode *root;
  Camera camera;
  World world;
  LightNode *light;

  Wumpus *wumpus;
};
#endif
