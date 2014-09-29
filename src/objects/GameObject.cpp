#include "GameObject.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

using namespace matc;

bool GameObject::init(std::string modelPath, std::string texture, std::string shader)
{
  transform = new TransformNode("Transform", translate(Matrix4x4(), Vector3(xPosition, 1.0, zPosition)));
  material = new MaterialNode("Material", texture, shader);
  material->specularIntensity = 0.3f;
  material->specularPower = 1.0f;
  model = new ModelNode(name, modelPath);
  //root->addChild(wumpusPosition);
  transform->addChild(material);
  material->addChild(model);

  return true;
}
