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
  Scene *scene;
  TransformNode *transform;
  MaterialNode *material;
  ModelNode *model;
};
#endif
