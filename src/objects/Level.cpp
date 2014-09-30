#include "Level.hpp"

#include "MathCore.hpp"
#include "TransformNode.hpp"
#include "ModelNode.hpp"
#include "MaterialNode.hpp"
#include "jsoncpp.h"

using namespace matc;

WallSegment::WallSegment(Vector3 position, Vector3 rotation)
{
  Matrix4x4 xRotation = rotate(Matrix4x4(), rotation.x, Vector3(1.0, 0.0, 0.0));
  Matrix4x4 yxRotation = rotate(xRotation, rotation.y, Vector3(0.0, 1.0, 0.0));
  Matrix4x4 zyxRotation = rotate(yxRotation, rotation.z, Vector3(0.0, 0.0, 1.0));
  Matrix4x4 transformMatrix = translate(zyxRotation, position);
  transform = new TransformNode("WallSegment-Transform", transformMatrix);
  material = new MaterialNode("WallSegment-Material", "wall_segment", "phongShader");//"texturedShader");
  //material->specularIntensity = 0.1f;
  //material->specularPower = 1.0f;
  model = new ModelNode("WallSegment-Model", "../assets/wall_segment.obj");

  scaleTrafo = new TransformNode("WallSegment-Scaling", scale(Matrix4x4(), CELL_SIZE/2, 2.0f, 1.0f));

  transform->addChild(material);
  material->addChild(scaleTrafo);
  scaleTrafo->addChild(model);
}

bool WallSegment::linkPart(SceneNode& node)
{
  node.addChild(this->transform);
  return true;
}

FloorTile::FloorTile(Vector3 position)
{
  transform = new TransformNode("Floor", translate(Matrix4x4(), position));
  material = new MaterialNode("Floor-Material", "floor", "phongShader");//"texturedShader");
  //material->specularIntensity = 0.1f;
  //material->specularPower = 1.0f;
  model = new ModelNode("Floor-Model", "../assets/floor.obj");
  scaleTrafo = new TransformNode("Floor-Scaling", scale(Matrix4x4(), CELL_SIZE/2, 5.0f, CELL_SIZE/2));
  
  transform->addChild(material);
  material->addChild(scaleTrafo);
  scaleTrafo->addChild(model);
}

bool FloorTile::linkPart(SceneNode& node)
{
  node.addChild(this->transform);
  return true;
}

PitTile::PitTile(Vector3 position)
{
  transform = new TransformNode("Pit", translate(Matrix4x4(), position));
  material = new MaterialNode("Pit-Material", "pit", "phongShader");//"texturedShader");
  //material->specularIntensity = 0.1f;
  //material->specularPower = 1.0f;
  model = new ModelNode("Pit-Model", "../assets/floor.obj");
  scaleTrafo = new TransformNode("Pit-Scaling", scale(Matrix4x4(), CELL_SIZE/2, 5.0f, CELL_SIZE/2));
  
  transform->addChild(material);
  material->addChild(scaleTrafo);
  scaleTrafo->addChild(model);
}

bool PitTile::linkPart(SceneNode& node)
{
  node.addChild(this->transform);
  return true;
}

Level::Level(int width, int height, std::vector<Json::Value> pitValues)
{
  transform = new TransformNode("World", Matrix4x4());// translate(Matrix4x4(),
				//Vector3(-(CELL_SIZE*width)/2.0, 0.0, (CELL_SIZE*height)/2.0)));
  //floor.linkPart(*transform);

  int minZ = -height*CELL_SIZE;
  //int maxZ = 6*height;
  //int minX = -(6*width);
  int maxX = width*CELL_SIZE;

  for(int z = 0; z < height; z += 1) {
    Vector3 pos(0, 0, -z*CELL_SIZE + CELL_SIZE/2);
      walls.push_back(new WallSegment(pos, Vector3(0.0, 3.14f/2, 0.0)));
  }
  for(int x = 0; x < width; x += 1) {
    Vector3 pos(x*CELL_SIZE + CELL_SIZE/2, 0, CELL_SIZE);
    walls.push_back(new WallSegment(pos, Vector3(0.0, 0, 0.0)));
  }
  for(int z = 0; z < height; z += 1) {
    Vector3 pos(maxX , 0, -z*CELL_SIZE + CELL_SIZE/2);
    walls.push_back(new WallSegment(pos, Vector3(0.0, 3.14f/2, 0.0)));
  }
  for(int x = 0; x < width; x += 1) {
    Vector3 pos(x*CELL_SIZE+CELL_SIZE/2, 0, minZ + CELL_SIZE);
    walls.push_back(new WallSegment(pos, Vector3(0.0, 0, 0.0)));
  }
  for(int i = 0; i < walls.size(); i++) {
    walls[i]->linkPart(*transform);
  }

  for(int z = 0; z < height; z += 1) {
    for(int x = 0; x < width; x += 1) {

      floor.push_back(new FloorTile(Vector3(gridToPos(x), 0, -gridToPos(z))));//x*CELL_SIZE + CELL_SIZE/2, 0, -z*CELL_SIZE + CELL_SIZE/2)));
    }
  }
  for(int i = 0; i < floor.size(); i++) {
    floor[i]->linkPart(*transform);
  }

  for(int i = 0; i < pitValues.size(); i++) {
    //std::cout << pitValues[i]["xpos"] << std::endl;
    int x = pitValues[i]["xpos"].asInt()-1;
    int z = pitValues[i]["zpos"].asInt()-1;

    pits.push_back(new PitTile(Vector3(gridToPos(x), 0.1, -gridToPos(z))));
				       //x*CELL_SIZE+CELL_SIZE/2, 0.1, -z*CELL_SIZE + CELL_SIZE/2)));
    pits[i]->linkPart(*transform);
  }
}

Level::~Level()
{
  transform->release();
  for(int i = 0; i < walls.size(); i++) {
    delete walls[i];
  }
  for(int i = 0; i < floor.size(); i++) {
    delete floor[i];
  }
  walls.clear();
  floor.clear();
  for(int i = 0; i < pits.size(); i++) {
    delete pits[i];
  }
  pits.clear();
}

bool Level::linkLevel(SceneNode& link)
{
  link.addChild(this->transform);
  return true;
}
