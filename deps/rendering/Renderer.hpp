#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <stack>
#include "NodeVisitor.hpp"
#include "RenderContext.hpp"
#include "OGLRenderEngine.hpp"

class Renderer: public NodeVisitor
{
public:
  Renderer(OGLRenderEngine* engine);
  ~Renderer() {}

  bool visit(ModelNode& model);
  bool visit(CubeNode& cubeNode);
  bool visit(TransformNode& transform);
  bool visit(CameraNode& camera);
  bool visit(LightNode& light);
  bool visit(MaterialNode& material);

  bool leave(TransformNode& transform);

private:
  bool cameraFound = false;
  RenderContext context;
  OGLRenderEngine* renderEngine;
  std::stack<matc::Matrix4x4> matrixStack;
};
#endif
