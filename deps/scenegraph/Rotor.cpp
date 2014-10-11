#include "Rotor.hpp"

#include "NodeVisitor.hpp"

Rotor::Rotor(std::string name, float angle, matc::Vector3 axis, float speed): TransformNode(name, matc::Matrix4x4()), cyclePerSecond(speed)
{
  cycleRotation = matc::rotate(matc::Matrix4x4(), angle, axis);
}

Rotor::~Rotor()
{
}

bool Rotor::accept(NodeVisitor& visitor)
{
  return TransformNode::accept(visitor);
}

void Rotor::update(float deltaTime)
{
  transform = cycleRotation*transform;
}
