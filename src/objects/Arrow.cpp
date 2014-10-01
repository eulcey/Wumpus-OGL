#include "Arrow.hpp"

Arrow::Arrow(float xPos, float zPos): GameObject("Arrow", xPos, zPos)
{
  init("../assets/arrow.obj", "arrow", "phongShader");
}

Arrow::~Arrow()
{
}

bool Arrow::release()
{
  return true;
}
