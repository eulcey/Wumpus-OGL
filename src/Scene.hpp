#ifndef SCENE_HPP
#define SCENE_HPP

#include "TransformNode.hpp"
#include "Camera.hpp"

class Scene
{
public:
  Scene(int width, int height);
  ~Scene() {}

private:
  TransformNode *root;
  Camera camera;
};
#endif
