#include "GameObject.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

using namespace matc;

bool GameObject::init(std::string modelPath, std::string texture, std::string shader)
{
  transform = new TransformNode("Transform", translate(Matrix4x4(), Vector3(xPosition, 3.5f, zPosition)));
  rotation = new TransformNode("Rotation", Matrix4x4());
  scale = new TransformNode("Scale", matc::scale(Matrix4x4(), 4.0f, 4.0f, 4.0f));
  material = new MaterialNode("Material", texture, shader);
  material->specularIntensity = 0.3f;
  material->specularPower = 1.0f;
  model = new ModelNode(name, modelPath);
  transform->addChild(rotation);
  rotation->addChild(material);
  material->addChild(scale);
  scale->addChild(model);

  return true;
}

void GameObject::setPosition(float xPos, float zPos)
{
  this->xPosition = xPos; this->zPosition = zPos;

  transform->setTransform(translate(Matrix4x4(), Vector3(xPosition, 3.5f, zPosition)));
}

void GameObject::rotate90Right()
{
  auto oldRotation = rotation->getTransform();
  rotation->setTransform(rotate(oldRotation, -3.14f/2.0f, Vector3(0, 1.0, 0)));
}

void GameObject::rotate90Left()
{
  auto oldRotation = rotation->getTransform();
  rotation->setTransform(rotate(oldRotation, 3.14f/2.0f, Vector3(0, 1.0, 0)));
}

Vector3 GameObject::getPosition()
{
  const float* values = transform->getTransform().asArray();
  float x = values[MAT_DIM * (MAT_DIM-1) + 0];
  float y = values[MAT_DIM * (MAT_DIM-1) + 1];
  float z = values[MAT_DIM * (MAT_DIM-1) + 2];
  return Vector3(x, y, z);
}
