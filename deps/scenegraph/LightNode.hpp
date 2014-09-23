#ifndef LIGHTNODE_HPP
#define LIGHTNODE_HPP

#include "SceneNode.hpp"
#include "MathCore.hpp"

enum LightingType {
  PointLight,
  DirectionLight
};

class LightNode: public SceneNode
{
public:
  LightNode(std::string name, LightingType type);
  ~LightNode() {}

  virtual bool accept(NodeVisitor &visitor);

  LightingType type;
  matc::Vector3 color;
  float ambientIntensity;
  matc::Vector3 direction;
  float diffuseIntensity;
private:
};
#endif
