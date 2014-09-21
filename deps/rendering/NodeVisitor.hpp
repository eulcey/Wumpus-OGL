#ifndef NODEVISITOR_HPP
#define NODEVISITOR_HPP

#include "ModelNode.hpp"
#include "CubeNode.hpp"
#include "TransformNode.hpp"
#include "CameraNode.hpp"
#include "LightNode.hpp"
#include "MaterialNode.hpp"

class NodeVisitor
{
public:
  NodeVisitor() {}
  virtual ~NodeVisitor() {}
  
  virtual bool visit(ModelNode& model) = 0;
  virtual bool visit(CubeNode& cube) = 0;
  virtual bool visit(TransformNode& transform) = 0;
  virtual bool visit(CameraNode& camera) = 0;
  virtual bool visit(LightNode& light) = 0;
  virtual bool leave(TransformNode& transform) = 0;
  virtual bool visit(MaterialNode& material) = 0;
};

#endif
