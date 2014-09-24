#ifndef CURSOR_HPP
#define CURSOR_HPP

class SceneNode;
class TransformNode;
class MaterialNode;
class ModelNode;

const float CURSOR_SPEED = 0.01;

class Cursor
{
public:
  Cursor();
  ~Cursor();

  void translatePosition(float xPos, float yPos);

  bool link(SceneNode& node);
  bool unlink(SceneNode& node);
 private:
  TransformNode *position;
  TransformNode *transform;
  MaterialNode *material;
  ModelNode *model;
};

#endif
