#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"
#include "jsoncpp-forwards.h"

const int CELL_SIZE = 12;

#define gridToPos(x) (x*CELL_SIZE)+CELL_SIZE/2
#define posToGrid(x) (x-CELL_SIZE/2)/CELL_SIZE

class LevelPart
{
public:
  LevelPart() {}
  virtual ~LevelPart() { transform->release(); material->release(); model->release(); scaleTrafo->release();}

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

class FloorTile: public LevelPart
{
public:
  FloorTile(matc::Vector3 position);
  ~FloorTile() {}

  virtual bool linkPart(SceneNode &link);
};

class PitTile: public LevelPart
{
public:
  PitTile(matc::Vector3 position);
  ~PitTile() {}

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
  std::vector<FloorTile*> floor;
  std::vector<PitTile*> pits;
  //std::vector<TransformNode*> pits;
};

#endif
