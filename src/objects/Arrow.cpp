#include "Arrow.hpp"

#include "MaterialNode.hpp"
#include "Rotor.hpp"
#include "Spring.hpp"

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

void Arrow::shoot(matc::Matrix4x4 pose)
{
  material->removeChild(*scale);
  Rotor *rotor = new Rotor("Arrow-Rotor", 3.14f/100.0f, matc::Vector3(0,0,1), 1);
  spring = new Spring("Arrow-Spring", matc::Vector3(0, 0, 0), matc::Vector3(0, 0, -10), 0.001f);
  rotor->addChild(scale);
  spring->addChild(rotor);
  material->addChild(spring);
}

bool Arrow::isAlive()
{
  if(!alive)
    return false;
  else {
    if (spring != 0 && spring->percentToEnd() < 0.3f) {
      alive = false;
    }
    return alive;
  }
  return alive;
}

void Arrow::hide()
{
  material->removeChild(*scale);
}
