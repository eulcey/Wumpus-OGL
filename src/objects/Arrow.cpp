#include "Arrow.hpp"

#include "MaterialNode.hpp"
#include "Rotor.hpp"

Arrow::Arrow(float xPos, float zPos): GameObject("Arrow", xPos, zPos)
{
  init("../assets/arrow.obj", "arrow", "phongShader");
  scale->setTransform(matc::rotate(matc::scale(matc::Matrix4x4(), 0.75f, 0.75f, 0.75f), -3.14f/2.0f, matc::Vector3(0, 1, 0)));
}

Arrow::~Arrow()
{
}

bool Arrow::release()
{
  return true;
}

void Arrow::shoot()
{
  material->removeChild(*scale);
  Rotor *rotor = new Rotor("Arrow-Rotor", 3.14f/100.0f, matc::Vector3(0,0,1), 1);
  rotor->addChild(scale);
  material->addChild(rotor);
}
