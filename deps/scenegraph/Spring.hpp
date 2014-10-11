#ifndef SPRING_HPP
#define SPRING_HPP

#include "TransformNode.hpp"

#include "MathCore.hpp"

class Spring: public TransformNode
{
public:
  Spring(std::string name, matc::Vector3 start, matc::Vector3 end, float speed);
  ~Spring();

  virtual bool accept(NodeVisitor& visitor);
  virtual void update(float deltaTime);

  bool isMoving = true;

private:
  matc::Vector3 startVec;
  matc::Vector3 endVec;
  float unitPerSecond;
  matc::Vector3 cycleTranslation;
};

#endif
