#include "World.hpp"

#include "MathCore.hpp"
#include "TransformNode.hpp"
#include "ModelNode.hpp"
#include "MaterialNode.hpp"

using namespace matc;

WallSegment::WallSegment(Vector3 position, Vector3 rotation)
{
  Matrix4x4 xRotation = rotate(Matrix4x4(), rotation.x, Vector3(1.0, 0.0, 0.0));
  Matrix4x4 yxRotation = rotate(xRotation, rotation.y, Vector3(0.0, 1.0, 0.0));
  Matrix4x4 zyxRotation = rotate(yxRotation, rotation.z, Vector3(0.0, 0.0, 1.0));
  Matrix4x4 transformMatrix = translate(zyxRotation, position);
  transform = new TransformNode("WallSegment-Transform", transformMatrix);
  material = new MaterialNode("WallSegment-Material", "wall_segment", "texturedShader");
  model = new ModelNode("WallSegment-Model", "../assets/cube.obj");

  scaleTrafo = new TransformNode("WallSegment-Scaling", scale(Matrix4x4(), 2.0f, 4.0f, 0.5f));

  transform->addChild(material);
  material->addChild(scaleTrafo);
  scaleTrafo->addChild(model);
}

bool WallSegment::linkSegment(SceneNode& node)
{
  node.addChild(this->transform);
  return true;
}
