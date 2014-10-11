#include "Cursor2D.hpp"

#include "TransformNode.hpp"

#include "CursorRenderer.hpp"

using namespace matc;

Cursor2D::Cursor2D()
{
  position = new TransformNode("Cursor-Position", translate(Matrix4x4(), Vector3(0, 0, -5)));
  transform = new TransformNode("Cursor-Transform",
				rotate(
				       rotate(
					      scale(Matrix4x4(), 0.3f, 0.3f, 0.3f),
					      -3.14f/4, Vector3(0.0f, 0.0f, 1.0f)),
				       -3.14f/4, Vector3(0.0f, 1.0f, 0.0f)));

  position->addChild(transform);
  initCursor2D("../assets/cursor2D.png", "textShader");
}

Cursor2D::~Cursor2D()
{
  position->release();
  transform->release();
  cleanupCursor2D();
}

void Cursor2D::translatePosition(float xPos, float yPos)
{
  Matrix4x4 oldTransform = position->getTransform();
  if(xPos > -50 && xPos < 50 && yPos > -50 && yPos < 50) { // BUG prevention initially values are to high/low
    position->setTransform(translate(oldTransform, Vector3(xPos * CURSOR2D_SPEED, -yPos * CURSOR2D_SPEED, 0)));
    
    realPosX += xPos;
    realPosY -= yPos;
  }
}

void Cursor2D::drawCursor()
{
  drawCursor2D(realPosX, realPosY, 20);
}

bool Cursor2D::link(SceneNode &node)
{
  return true;
}

bool Cursor2D::unlink(SceneNode &node)
{
  return node.removeChild(*position);
}

Vector3 Cursor2D::getPosition()
{
  const float* values = position->getTransform().asArray();
  float x = values[MAT_DIM * (MAT_DIM-1) + 0];
  float y = values[MAT_DIM * (MAT_DIM-1) + 1];
  float z = values[MAT_DIM * (MAT_DIM-1) + 2];
  return Vector3(x, y, z);
}
