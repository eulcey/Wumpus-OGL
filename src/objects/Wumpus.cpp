#include "Wumpus.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

using namespace matc;

Wumpus::Wumpus()
{
  wumpusPosition = new TransformNode("Wumpus Position", translate(Matrix4x4(), Vector3(0.0, 4.0, 0.0)));
  wumpusScale = new TransformNode("Wumpus Scale", scale(Matrix4x4(), 4, 4, 4));
  wumpusMaterial = new MaterialNode("Wumpus Material", "wumpus", "phongShader");//"shadowShader");//
  wumpusMaterial->specularIntensity = 0.3f;
  wumpusMaterial->specularPower = 1.0f;
  wumpus = new ModelNode("Wumpus", "../assets/monkey.obj");
  //root->addChild(wumpusPosition);
  wumpusPosition->addChild(wumpusMaterial);
  wumpusMaterial->addChild(wumpusScale);
  wumpusScale->addChild(wumpus);
}

Wumpus::~Wumpus()
{
  wumpusScale->release();
  wumpusPosition->release();
  wumpusMaterial->release();
  wumpus->release();
}

bool Wumpus::link(SceneNode &node)
{
  node.addChild(wumpusPosition);
  return true;
}

bool Wumpus::unlink(SceneNode& link)
{
  return link.removeChild(*this->wumpusPosition);
}

bool Wumpus::setPosition(float xpos, float zpos)
{
  wumpusPosition->setTransform(translate(Matrix4x4(), Vector3(xpos, 4.0, zpos)));
  return true;
}
