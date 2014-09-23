#ifndef WORLD_HPP
#define WORLD_HPP

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

class WorldPart
{
public:
  WorldPart() {}
  ~WorldPart() { transform->release(); material->release(); model->release(); scaleTrafo->release();}

  virtual bool linkPart(SceneNode &link) = 0;
  
protected:
  TransformNode *transform;
  MaterialNode *material;
  ModelNode *model;
  TransformNode *scaleTrafo;
};

class WallSegment: public WorldPart
{
public:
  WallSegment(matc::Vector3 position, matc::Vector3 rotation);
  ~WallSegment() {}

  virtual bool linkPart(SceneNode &link);
};

class Floor: public WorldPart
{
public:
  Floor();
  ~Floor() {}

  virtual bool linkPart(SceneNode &link);
};

class World
{
public:
  World();
  ~World();

  bool linkWorld(SceneNode &link);
  bool loadFile(std::string file);

private:
  TransformNode *transform;
  std::vector<WallSegment*> walls;
  Floor floor;
};

//World loadWorld(std::string file);
#endif
