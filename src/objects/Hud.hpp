#ifndef HUD_HPP
#define HUD_HPP

class Scene;
class SceneNode;
class TransformNode;
class MaterialNode;
class ModelNode;

class Hud
{
public:
  Hud(Scene *scene);
  ~Hud();

  bool pressButton(float xpos, float ypos);

  bool linkToCamera(SceneNode& node);

private:
  Scene *scene = 0;
  TransformNode *scale = 0;
  TransformNode *position = 0;
  MaterialNode *material = 0;
  ModelNode *model = 0;
};
#endif
