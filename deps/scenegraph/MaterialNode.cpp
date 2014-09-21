#include "MaterialNode.hpp"
#include "NodeVisitor.hpp"

bool MaterialNode::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}
