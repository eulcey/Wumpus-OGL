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
