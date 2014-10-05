#ifndef SHADOW_RENDERER_HPP
#define SHADOW_RENDERER_HPP

#include <stack>
#include "NodeVisitor.hpp"
#include "RenderContext.hpp"
#include "OGLRenderEngine.hpp"

class ShadowRenderer: public NodeVisitor
{
public:
  ShadowRenderer(OGLRenderEngine* engine);
  ~ShadowRenderer() {}

  bool visit(ModelNode& model);
  bool visit(CubeNode& cubeNode);
  bool visit(TransformNode& transform);
  bool visit(CameraNode& camera);
  bool visit(LightNode& light);
  bool visit(MaterialNode& material);

  bool leave(TransformNode& transform);

private:
  bool lightFound = false;
  RenderContext context;
  OGLRenderEngine* renderEngine;
  std::stack<matc::Matrix4x4> matrixStack;
};

#endif
