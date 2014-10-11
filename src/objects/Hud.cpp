#include "Hud.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"
#include "Scene.hpp"

using namespace matc;

Hud::Hud(Scene *scene): scene(scene)
{
  //int width = scene->getScreenWidth();
  //int height = scene->getScreenHeight();
  //std::cout << "hud: " << std::endl;
  float deltaX = 2.5f;//((width*1.0f)/height) * 2.5f; //5.5f;
  float deltaY = 1.9f;
  //std::cout << "deltaX: " << deltaX << ", deltaY: " << deltaY << std::endl;
  position = new TransformNode("HUD-Position", translate(Matrix4x4(), Vector3(deltaX, -deltaY, -6)));
  scale = new TransformNode("HUD-Transform", matc::scale(Matrix4x4(), 1.5f, 1.5f, 1.0f));
  material = new MaterialNode("HUD-Material", "hud", "texturedShader"); //"ambientShader"); //"phongShader"); //
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
