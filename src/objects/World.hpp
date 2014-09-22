#ifndef WORLD_HPP
#define WORLD_HPP

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

class WallSegment
{
public:
  WallSegment(matc::Vector3 position, matc::Vector3 rotation);
  ~WallSegment() { }// transform->release(); material->release(); model->release();}

  bool linkSegment(SceneNode &link);
  
private:
  TransformNode *transform;
  MaterialNode *material;
  ModelNode *model;
  TransformNode *scaleTrafo;
};

class World
{
public:
  World();
  ~World() {}

private:
  std::vector<WallSegment> walls;
  void loadFile(std::string file);
};

World loadWorld(std::string file);
#endif
