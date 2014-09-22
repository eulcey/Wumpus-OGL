#ifndef TRANSFORMNODE_HPP
#define TRANSFORMNODE_HPP

#include "SceneNode.hpp"
#include "MathCore.hpp"

class TransformNode: public SceneNode
{
public:
  TransformNode(std::string name, const matc::Matrix4x4 trans): SceneNode(name), transform(trans){}
  ~TransformNode() {}

  void setTransform(const matc::Matrix4x4 trans)
  {
    transform = matc::Matrix4x4(trans);
  }

  inline matc::Matrix4x4 getTransform() const
  {
    return transform;
  }
  
  virtual bool accept(NodeVisitor& visitor);
  
protected:
  virtual void doStream(std::ostream&);
  
private:
  matc::Matrix4x4 transform;
};
#endif
