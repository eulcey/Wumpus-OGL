#include "World.hpp"

#include "MathCore.hpp"
#include "TransformNode.hpp"
#include "ModelNode.hpp"
#include "MaterialNode.hpp"

using namespace matc;

WallSegment::WallSegment(Vector3 position, Vector3 rotation)
{
  Matrix4x4 xRotation = rotate(Matrix4x4(), rotation.x, Vector3(1.0, 0.0, 0.0));
  Matrix4x4 yxRotation = rotate(xRotation, rotation.y, Vector3(0.0, 1.0, 0.0));
  Matrix4x4 zyxRotation = rotate(yxRotation, rotation.z, Vector3(0.0, 0.0, 1.0));
  Matrix4x4 transformMatrix = translate(zyxRotation, position);
  transform = new TransformNode("WallSegment-Transform", transformMatrix);
  material = new MaterialNode("WallSegment-Material", "wall_segment", "texturedShader");
  model = new ModelNode("WallSegment-Model", "../assets/wall_segment.obj");

  scaleTrafo = new TransformNode("WallSegment-Scaling", scale(Matrix4x4(), 1.0f, 2.0f, 1.0f));

  transform->addChild(material);
  material->addChild(scaleTrafo);
  scaleTrafo->addChild(model);
}

bool WallSegment::linkPart(SceneNode& node)
{
  node.addChild(this->transform);
  return true;
}

Floor::Floor()
{
  transform = new TransformNode("Floor", translate(Matrix4x4(), Vector3(0.0, 0.0, 0.0)));
  material = new MaterialNode("Floor-Material", "cube", "texturedShader");
  model = new ModelNode("Floor-Model", "../assets/cube.obj");
  scaleTrafo = new TransformNode("Floor-Scaling", scale(Matrix4x4(), 50.0f, 0.2f, 50.0f));
  
  transform->addChild(material);
  material->addChild(scaleTrafo);
  scaleTrafo->addChild(model);
}

bool Floor::linkPart(SceneNode& node)
{
  node.addChild(this->transform);
  return true;
}

World::World()
{
  transform = new TransformNode("World", Matrix4x4());
  floor.linkPart(*transform);
}

World::~World()
{
  transform->release();
  for(int i = 0; i < walls.size(); i++) {
    delete walls[i];
  }
  walls.clear();
}

bool World::loadFile(std::string file)
{
  /*
  WallSegment segment1(Vector3(-2.0, 0.0, 1.0), Vector3(0.0, 3.14f/2, 0.0));
  WallSegment segment2(Vector3(-2.0, 0.0, 3.0), Vector3(0.0, 3.14f/2, 0.0));
  WallSegment segment3(Vector3(-2.0, 0.0, 5.0), Vector3(0.0, 3.14f/2, 0.0));
  WallSegment segment4(Vector3(-2.0, 0.0, 7.0), Vector3(0.0, 3.14f/2, 0.0));
  */
  for(int z = -15; z < 25; z += 2) {
    walls.push_back(new WallSegment(Vector3(-2.0, 0.0, z), Vector3(0.0, 3.14f/2, 0.0)));
  }
  for(int i = 0; i < walls.size(); i++) {
    walls[i]->linkPart(*transform);
  }
  return true;
}

bool World::linkWorld(SceneNode& link)
{
  link.addChild(this->transform);
  return true;
}

/*
World loadWorld(std::string file)
{
  World world;
  if(!world.loadFile(file)) {
    std::cerr << "Couldn't load File: " << file << std::endl;
  }
  return world;
}
*/
