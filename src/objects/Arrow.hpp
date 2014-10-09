#ifndef ARROW_HPP
#define ARROW_HPP

#include "GameObject.hpp"
#include "MathCore.hpp"

class Arrow: public GameObject
{
public:
  Arrow(float xPos, float zPos);
  ~Arrow();

  virtual bool release();
  
  bool isAlive() { return alive; }
  bool collidesWithWall(float xPos, float zPos);
  void shoot();

private:
  bool alive = false;
};

#endif
