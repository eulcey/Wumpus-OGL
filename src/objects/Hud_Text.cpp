#include "Hud_Text.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"
#include "Scene.hpp"

using namespace matc;

Hud_Text::Hud_Text(Scene *scene): scene(scene)
{
  // int width = scene->getScreenWidth();
  //int height = scene->getScreenHeight();
  float deltaX = -2.8f;// -((width*1.0f)/height); // -2.8f
  float deltaY = 2.3f; // 2.3f
  //std::cout << "hudtext: " << deltaX << ", " << deltaY << std::endl;
  position = new TransformNode("HUD-Text-Position", translate(Matrix4x4(), Vector3(deltaX, deltaY, -6)));
  scale = new TransformNode("HUD-Text-Transform", matc::scale(Matrix4x4(), 0.8f, 0.6f, 1.0f));
  material = new MaterialNode("HUD-Text-Material", "hud_text","texturedShader"); //"ambientShader"); //"phongShader"); // 
  model = new ModelNode("HUD-Text-Model", "../assets/hud_text.obj");

  position->addChild(material);
  material->addChild(scale);
  scale->addChild(model);

}

Hud_Text::~Hud_Text()
{
  if(model != NULL)
    model->release();
  if(material != NULL)
    material->release();
  if(position != NULL)
    position->release();
}

bool Hud_Text::displayText(std::string line)
{
  return false;
}

bool Hud_Text::linkToCamera(SceneNode &node)
{
  node.addChild(position);
  return true;
}
