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
    //context.setCameraPosition(cNode.getPosition());
    const std::vector<float> viewValues = matrixStack.top().getValues();
    float xPos = viewValues[12];
    float yPos = viewValues[13];
    float zPos = viewValues[14];
    context.setCameraPosition(Vector3(xPos, yPos, zPos));
  }
  //std::cout << "rendering camera" << std::endl;
  return true;
}

bool Renderer::visit(LightNode& lNode)
{
  // std::cout << "rendering light" << std::endl;
  Light light;
  light.color = lNode.color;
  light.ambientIntensity = lNode.ambientIntensity;
  light.direction = lNode.direction;
  light.diffuseIntensity = lNode.diffuseIntensity;
  context.setLight(light);
  return true;
}

bool Renderer::visit(MaterialNode& material)
{
  context.setTexture(material.getTexture());
  context.setShader(material.getShader());
  context.specularIntensity = material.specularIntensity;
  context.specularPower = material.specularPower;
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
