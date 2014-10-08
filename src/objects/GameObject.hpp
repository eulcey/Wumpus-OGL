#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "TransformNode.hpp"

class MaterialNode;
class ModelNode;

class GameObject
{
public:
  GameObject(std::string name, float xPos, float zPos): name(name), xPosition(xPos), zPosition(zPos) {}
  virtual ~GameObject() {}

  virtual void setPosition(float xPos, float zPos);
  virtual bool init(std::string model, std::string texture, std::string shader);
  virtual bool release() = 0;
  virtual void rotate90Right();
  virtual void rotate90Left();
  matc::Vector3 getPosition();

  bool link(SceneNode& node) { node.addChild(transform); return true; }
  bool unlink(SceneNode &node) { return node.removeChild(*transform); }
  TransformNode* getPose() { return rotation; }
protected:
  std::string name;
  float xPosition;
  float zPosition;
  TransformNode *transform;
  TransformNode *rotation;
  TransformNode *scale;
  MaterialNode *material;
  ModelNode *model;
};

#endif
