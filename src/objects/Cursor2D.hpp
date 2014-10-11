#ifndef CURSOR2D_HPP
#define CURSOR2D_HPP

#include "MathCore.hpp"

class SceneNode;
class TransformNode;
class MaterialNode;
class ModelNode;

const float CURSOR2D_SPEED = 0.01;

class Cursor2D
{
public:
  Cursor2D();
  ~Cursor2D();

  void translatePosition(float xPos, float yPos);
  matc::Vector3 getPosition();

  bool link(SceneNode& node);
  bool unlink(SceneNode& node);

  void drawCursor();
  
  bool active = false;
 private:
  TransformNode *position;
  TransformNode *transform;

  float realPosX = 0.0f;
  float realPosY = 0.0f;
};

#endif
