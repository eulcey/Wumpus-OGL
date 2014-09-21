#include "TransformNode.hpp"
#include "NodeVisitor.hpp"

bool TransformNode::accept(NodeVisitor& visitor)
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
    return visitor.leave(*this);
  }
}

void TransformNode::doStream(std::ostream& os)
{
  SceneNode::doStream(os);
  //os << "\n" << transform << "\n";
}
