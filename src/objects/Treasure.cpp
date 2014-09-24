#include "Treasure.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

#include "MathCore.hpp"

using namespace matc;

Treasure::Treasure(float xPos, float zPos): GameObject(xPos, zPos)
{
  transform = new TransformNode("Treasure-Position", translate(Matrix4x4(), Vector3(xPos, zPos, 0.0)));
  material = new MaterialNode("Treasure-Material", "chest", "phongShader");
  material->specularIntensity = 3.0f;
  material->specularPower = 1.0f;
  model = new ModelNode("Treasure-Model", "../assets/ball.obj");
  transform->addChild(material);
  material->addChild(model);
}

Treasure::~Treasure()
{
  this->release();
}

bool Treasure::setPosition(float xPos, float yPos)
{
  transform->setTransform(translate(Matrix4x4(), Vector3(xPos, yPos, 0)));
  this->xPosition = xPos;
  this->zPosition = yPos;
}

bool Treasure::init()
{
  
}

bool Treasure::release()
{
  model->release();
  material->release();
  transform->release();
}
