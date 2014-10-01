#include "Hud.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

using namespace matc;

Hud::Hud(Scene *scene): scene(scene)
{
  position = new TransformNode("HUD-Position", translate(Matrix4x4(), Vector3(2.5f, -1.9f, -6)));
  scale = new TransformNode("HUD-Transform", matc::scale(Matrix4x4(), 1.5f, 1.5f, 1.0f));
  material = new MaterialNode("HUD-Material", "hud", "ambientShader");
  model = new ModelNode("HUD-Model", "../assets/hud.obj");

  position->addChild(material);
  material->addChild(scale);
  scale->addChild(model);

}

Hud::~Hud()
{
  if(model != NULL)
    model->release();
  if(material != NULL)
    material->release();
  if(position != NULL)
    position->release();
}

bool Hud::pressButton(float xpos, float ypos)
{
  return false;
}

bool Hud::linkToCamera(SceneNode &node)
{
  node.addChild(position);
  return true;
}
