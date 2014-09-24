#include "RenderContext.hpp"
#include "ModelNode.hpp"
#include <iostream>

using namespace matc;

/*
  bool RenderContext::render(ModelNode& model)
{
  std::cout << "rendering model: " << model << std::endl;
  std::cout << "with projection: " << projectionTransformation << std::endl;
  std::cout << "with view: " << viewTransformation << std::endl;
  std::cout << "with model: " << modelTransformation << std::endl;
  std::cout << std::endl;
}
*/

void RenderContext::setProjection(Matrix4x4 proj)
{
  this->projectionTransformation = proj;
}

void RenderContext::setView(Matrix4x4 view)
{
  this->viewTransformation = view;
}

void RenderContext::setModel(Matrix4x4 model)
{
  this->modelTransformation = model;
}

Vector3 RenderContext::getCameraPosition() const
{
  const std::vector<float> viewValues = viewTransformation.getValues();
  float xPos = viewValues[12];
  float yPos = viewValues[13];
  float zPos = viewValues[14];
  //Matrix4x4 test(viewTransformation);
  //std::cout << "view: " << test << std::endl;
  //std::cout << "camera pos at: " << xPos << ", " << yPos << ", " << zPos << std::endl;
  return Vector3(xPos, yPos, zPos);
}
