#ifndef ARROW_HPP
#define ARROW_HPP

#include "GameObject.hpp"
#include "MathCore.hpp"

class Spring;

class Arrow: public GameObject
{
public:
  Arrow(float xPos, float zPos);
  ~Arrow();

  virtual bool release();
  
  bool isAlive();
  bool collidesWithWall(float xPos, float zPos);
  void shoot(matc::Matrix4x4 pose);

  void hide();

private:
  bool alive = true;
  Spring *spring = 0;
};

#endif
