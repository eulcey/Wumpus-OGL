#ifndef ROTOR_HPP
#define ROTOR_HPP

#include "TransformNode.hpp"

#include "MathCore.hpp"

class Rotor: public TransformNode
{
public:
  Rotor(std::string name, float angle, matc::Vector3 axis, float speed);
  ~Rotor();

  virtual bool accept(NodeVisitor& visitor);
  virtual void update(float deltaTime);

  bool isRotating = true;

private:
  float cyclePerSecond;
  matc::Matrix4x4 cycleRotation;
};

#endif
