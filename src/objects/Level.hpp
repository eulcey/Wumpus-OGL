#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"
#include "jsoncpp-forwards.h"

const int CELL_SIZE = 12;

class LevelPart
{
public:
  LevelPart() {}
  ~LevelPart() { transform->release(); material->release(); model->release(); scaleTrafo->release();}

  virtual bool linkPart(SceneNode &link) = 0;
  
protected:
  TransformNode *transform;
  MaterialNode *material;
  ModelNode *model;
  TransformNode *scaleTrafo;
};

class WallSegment: public LevelPart
{
public:
  WallSegment(matc::Vector3 position, matc::Vector3 rotation);
  ~WallSegment() {}

  virtual bool linkPart(SceneNode &link);
};

class Floor: public LevelPart
{
public:
  Floor();
  ~Floor() {}

  virtual bool linkPart(SceneNode &link);
};


class Level
{
public:
  Level(int width, int height, std::vector<Json::Value> pitValues);
  ~Level();

  bool linkLevel(SceneNode &link);
  //bool loadFile(std::string file);

private:
  TransformNode *transform;
  std::vector<WallSegment*> walls;
  Floor floor;
};

#endif
