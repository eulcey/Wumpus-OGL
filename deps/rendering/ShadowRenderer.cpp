#include "ShadowRenderer.hpp"
#include <iostream>

using namespace matc;

ShadowRenderer::ShadowRenderer(OGLRenderEngine* engine): renderEngine(engine)
{
  matrixStack.push(Matrix4x4());
  context = RenderContext();
}

bool ShadowRenderer::visit(ModelNode& mNode)
{
  context.setModel(matrixStack.top());
  renderEngine->renderShadow(context, mNode);
  //std::cout << "rendering model" << std::endl;
  return true;
}

bool ShadowRenderer::visit(CubeNode& cubeNode)
{
  context.setModel(matrixStack.top());
  renderEngine->renderShadow(context, cubeNode);
  //std::cout << "rendering cube" << std::endl;
  return true;
}

bool ShadowRenderer::visit(TransformNode& tNode)
{
  Matrix4x4 transform = tNode.getTransform();
  Matrix4x4 top = matrixStack.top();
  matrixStack.push(top * transform);
  //std::cout << "renderer visiting transform" << std::endl;
  return true;
}

bool ShadowRenderer::visit(CameraNode& cNode)
{
  /*
  if(cNode.isActive()) {
    cameraFound = true;
    Matrix4x4 view = matrixStack.top().invert();
    context.setView(view);
    context.setProjection(cNode.getProjection());
    //context.setCameraPosition(cNode.getPosition());
  }
  //std::cout << "rendering camera" << std::endl;
  */
  return true;
}

bool ShadowRenderer::visit(LightNode& lNode)
{
  /*
  // std::cout << "rendering light" << std::endl;
  Light light;
  light.color = lNode.color;
  light.ambientIntensity = lNode.ambientIntensity;
  light.direction = lNode.direction;
  light.diffuseIntensity = lNode.diffuseIntensity;
  context.setLight(light);
  */
  lightFound = true;
  Matrix4x4 view = matc::lookAt(lNode.direction * -1, Vector3(0, 0, 0), Vector3(0, 1, 0));
  context.setView(view);
  Matrix4x4 projection = matc::ortho(-10, 10, -10, 10, -10, 20);
  context.setProjection(projection);
  return true;
}

bool ShadowRenderer::visit(MaterialNode& material)
{
  /*
  context.setTexture(material.getTexture());
  context.setShader(material.getShader());
  context.specularIntensity = material.specularIntensity;
  context.specularPower = material.specularPower;
  */
  //context.setVertexShader(material.getVertexShader());
  //context.setFragmentShader(material.getFragmentShader());
  return true;
}

bool ShadowRenderer::leave(TransformNode& tNode)
{
  //std::cout << "renderer leaving transform" << std::endl;
  matrixStack.pop();
  return true;
}
