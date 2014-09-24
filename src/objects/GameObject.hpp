#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "TransformNode.hpp"

class MaterialNode;
class ModelNode;

class GameObject
{
public:
  GameObject(float xPos, float zPos): xPosition(xPos), zPosition(zPos) {}
  ~GameObject() {}

  virtual bool setPosition(float xPos, float zPos) { this->xPosition = xPos; this->zPosition = zPos; }
  virtual bool init() = 0;
  virtual bool release() = 0;

  bool link(SceneNode& node) { node.addChild(transform); return true; }
  bool unlink(SceneNode &node) { return node.removeChild(*transform); }
protected:
  float xPosition;
  float zPosition;
  TransformNode *transform;
  MaterialNode *material;
  ModelNode *model;
};

#endif
