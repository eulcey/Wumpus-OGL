#include "MaterialNode.hpp"
#include "NodeVisitor.hpp"

bool MaterialNode::accept(NodeVisitor& visitor)
{
  if(visitor.visit(*this)) {
    for(std::list<SceneNode*>::iterator i = lstChildren.begin();
	i != lstChildren.end();
	i++)
      {
	if(!(*i)->accept(visitor)) {
	  std::cerr << "Error in traversel of: " << *this << std::endl;
	  break;
	}
      }
  }
  return true;
}
