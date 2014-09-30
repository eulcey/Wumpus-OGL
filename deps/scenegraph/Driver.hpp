#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "TransformNode.hpp"
#include "MathCore.hpp"

enum Direction {
  FORWARD,
  BACK,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

class Driver: public TransformNode
{
public:
  Driver(std::string name, matc::Vector3 direction, matc::Vector3 right);
  ~Driver();

  void update(float deltaTime);

  void setDirection(Direction direction);

private:
  matc::Matrix4x4 position;
  matc::Vector3 direction;
  matc::Vector3 right;
  float speed;
};

#endif
