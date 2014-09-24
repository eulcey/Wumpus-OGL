#include "Hud.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

Hud::Hud(Scene *scene): scene(scene)
{
}

Hud::~Hud()
{
  model->release();
  material->release();
  transform->release();
}

bool Hud::pressButton(float xpos, float ypos)
{
  return false;
}

bool Hud::linkToCamera(SceneNode &node)
{
  node.addChild(transform);
  return true;
}
