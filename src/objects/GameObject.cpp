#include "GameObject.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

using namespace matc;

bool GameObject::init(std::string modelPath, std::string texture, std::string shader)
{
  transform = new TransformNode("Transform", translate(Matrix4x4(), Vector3(xPosition, 3.5f, zPosition)));
  scale = new TransformNode("Scale", matc::scale(Matrix4x4(), 4.0f, 4.0f, 4.0f));
  material = new MaterialNode("Material", texture, shader);
  material->specularIntensity = 0.3f;
  material->specularPower = 1.0f;
  model = new ModelNode(name, modelPath);
  //root->addChild(wumpusPosition);
  transform->addChild(material);
  material->addChild(scale);
  scale->addChild(model);

  return true;
}

void GameObject::setPosition(float xPos, float zPos)
{
  this->xPosition = xPos; this->zPosition = zPos;

  transform->setTransform(translate(Matrix4x4(), Vector3(xPosition, 3.5f, zPosition)));
}
