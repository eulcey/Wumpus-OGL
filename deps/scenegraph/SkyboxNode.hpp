#ifndef SkYBOXNODE_HPP
#define SKYBOXNODE_HPP

#include <vector>
#include "MathCore.hpp"
#include "SceneNode.hpp"

class SkyboxNode: public SceneNode
{
public:
  SkyboxNode(std::string name, std::vector<std::string> textureNames, std::string shader);
  ~SkyboxNode() {}

  bool accept(NodeVisitor& visitor);
  void update() {}

private:
  
};

#endif
