#include "LightNode.hpp"
#include "NodeVisitor.hpp"

LightNode::LightNode(std::string name, LightingType type): SceneNode(name), type(type)
{
  color = matc::Vector3(1.0, 1.0, 1.0);
  ambientIntensity = 1.0f;
  direction = matc::Vector3(0.0, -1.0, 0.0);
  diffuseIntensity = 1.0f;
}

bool LightNode::accept(NodeVisitor &visitor)
{
  visitor.visit(*this);
}
