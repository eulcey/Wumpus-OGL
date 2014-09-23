#include "Wumpus.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

using namespace matc;

Wumpus::Wumpus()
{
  wumpusPosition = new TransformNode("Wumpus Position", translate(Matrix4x4(), Vector3(0.0, 4.0, 0.0)));
  wumpusMaterial = new MaterialNode("Wumpus Material", "wumpus", "ambientShader");
  wumpus = new ModelNode("Wumpus", "../assets/monkey.obj");
  //root->addChild(wumpusPosition);
  wumpusPosition->addChild(wumpusMaterial);
  wumpusMaterial->addChild(wumpus);
}

Wumpus::~Wumpus()
{ 
  wumpusPosition->release();
  wumpusMaterial->release();
  wumpus->release();
}

bool Wumpus::link(SceneNode &node)
{
  node.addChild(wumpusPosition);
}

bool Wumpus::setPosition(float xpos, float zpos)
{
  wumpusPosition->setTransform(translate(Matrix4x4(), Vector3(xpos, 2.0, zpos)));
}
