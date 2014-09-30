#ifndef JUMPER_HPP
#define JUMPER_HPP

#include "TransformNode.hpp"

#include "MathCore.hpp"

class Jumper: public TransformNode
{
public:
  Jumper(std::string name, matc::Vector3 start, matc::Vector3 end);
  ~Jumper();

private:
  matc::Vector3 start;
  matc::Vector3 end;
  matc::Vector3 actual;
};

#endif
