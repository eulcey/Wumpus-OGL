#include "Driver.hpp"

#include "MathCore.hpp"

using namespace matc;

Driver::Driver(std::string name, matc::Vector3 direction, matc::Vector3 right): TransformNode(name, Matrix4x4())
{
  
}

Driver::~Driver()
{
}

void Driver::update(float deltaTime)
{
}

void Driver::changeHorizAngle(float horizAngle)
{
}

void Driver::changeVertAngle(float vertAngle)
{
}
