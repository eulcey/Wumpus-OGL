#include "Renderer.hpp"
#include <iostream>

using namespace matc;

Renderer::Renderer(OGLRenderEngine* engine): renderEngine(engine)
{
  matrixStack.push(Matrix4x4());
  context = RenderContext();
}

bool Renderer::visit(ModelNode& mNode)
{
  context.setModel(matrixStack.top());
  renderEngine->render(context, mNode);
  //std::cout << "rendering model" << std::endl;
  return true;
}

bool Renderer::visit(CubeNode& cubeNode)
{
  context.setModel(matrixStack.top());
  renderEngine->render(context, cubeNode);
  //std::cout << "rendering cube" << std::endl;
  return true;
}

bool Renderer::visit(TransformNode& tNode)
{
  Matrix4x4 transform = tNode.getTransform();
  Matrix4x4 top = matrixStack.top();
  matrixStack.push(top * transform);
  //std::cout << "renderer visiting transform" << std::endl;
  return true;
}

bool Renderer::visit(CameraNode& cNode)
{
  if(cNode.isActive()) {
    cameraFound = true;
    Matrix4x4 view = matrixStack.top().invert();
    context.setView(view);
    context.setProjection(cNode.getProjection());
  }
  //std::cout << "rendering camera" << std::endl;
  return true;
}

bool Renderer::visit(LightNode& lNode)
{
  // std::cout << "rendering light" << std::endl;
  return true;
}

bool Renderer::visit(MaterialNode& material)
{
  context.setTexture(material.getTexture());
  context.setShader(material.getShader());
  //context.setVertexShader(material.getVertexShader());
  //context.setFragmentShader(material.getFragmentShader());
  return true;
}

bool Renderer::leave(TransformNode& tNode)
{
  //std::cout << "renderer leaving transform" << std::endl;
  matrixStack.pop();
  return true;
}
