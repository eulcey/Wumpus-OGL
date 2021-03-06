#include "Cursor.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"

using namespace matc;

Cursor::Cursor()
{
  position = new TransformNode("Cursor-Position", translate(Matrix4x4(), Vector3(0, 0, -5)));
  transform = new TransformNode("Cursor-Transform",
				rotate(
				       rotate(
					      scale(Matrix4x4(), 0.3f, 0.3f, 0.3f),
					      -3.14f/4, Vector3(0.0f, 0.0f, 1.0f)),
				       -3.14f/4, Vector3(0.0f, 1.0f, 0.0f)));
  material = new MaterialNode("Cursor-Material", "cursor", "phongShader"); //"ambientShader");
  model = new ModelNode("Cursor-Model", "../assets/cursor.obj");

  position->addChild(material);
  material->addChild(transform);
  transform->addChild(model);
}

Cursor::~Cursor()
{
  position->release();
  transform->release();
  material->release();
  model->release();
}

void Cursor::translatePosition(float xPos, float yPos)
{
  Matrix4x4 oldTransform = position->getTransform();
  if(xPos > -50 && xPos < 50 && yPos > -50 && yPos < 50) { // BUG prevention initially values are to high/low
    position->setTransform(translate(oldTransform, Vector3(xPos * CURSOR_SPEED, -yPos * CURSOR_SPEED, 0)));
  }
}

bool Cursor::link(SceneNode &node)
{
  node.addChild(position);
  std::cout << "Cursor linking to: " << node << std::endl;
  return true;
}

bool Cursor::unlink(SceneNode &node)
{
  return node.removeChild(*position);
}

Vector3 Cursor::getPosition()
{
  const float* values = position->getTransform().asArray();
  float x = values[MAT_DIM * (MAT_DIM-1) + 0];
  float y = values[MAT_DIM * (MAT_DIM-1) + 1];
  float z = values[MAT_DIM * (MAT_DIM-1) + 2];
  return Vector3(x, y, z);
}
