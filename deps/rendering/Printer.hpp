#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <iostream>
#include "NodeVisitor.hpp"

class Printer : public NodeVisitor 
{
public:
  Printer() {}
  ~Printer() {}
  
  bool visit(ModelNode& model);
  bool visit(CubeNode& cube);
  bool visit(TransformNode& transform);
  bool visit(CameraNode& camera);
  bool visit(LightNode& light);
  bool visit(MaterialNode& material);

  bool leave(TransformNode& transform);

private:
  int printTab(int);
  int count = 0;
};
#endif
