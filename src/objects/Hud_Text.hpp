#ifndef HUD_TEXT_HPP
#define HUD_TEXT_HPP

#include <string>

class Scene;
class SceneNode;
class TransformNode;
class MaterialNode;
class ModelNode;

class Hud_Text
{
public:
  Hud_Text(Scene *scene);
  ~Hud_Text();

  bool displayText(std::string line);
  bool linkToCamera(SceneNode& node);

  
  TransformNode *scale = 0;
  TransformNode *position = 0;

private:
  Scene *scene = 0;  MaterialNode *material = 0;
  ModelNode *model = 0;
};
#endif
