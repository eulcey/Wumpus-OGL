#include "CameraNode.hpp"
#include "NodeVisitor.hpp"
#include "MathCore.hpp"

using namespace matc;

bool CameraNode::accept(NodeVisitor& visitor)
{
  return visitor.visit(*this);
}

Matrix4x4 CameraNode::getProjection()
{
  return perspective(fov, aspectRatio, nearPlane, farPlane);
}
