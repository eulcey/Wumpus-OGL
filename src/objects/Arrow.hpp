#ifndef ARROW_HPP
#define ARROW_HPP

#include "GameObject.hpp"

class Arrow: public GameObject
{
public:
  Arrow(float xPos, float zPos);
  ~Arrow();

  virtual bool release();
  
  bool isAlive();
  void getPosition(float &xPos, float zPos);
  bool collidesWithWall(float xPos, float zPos);

private:
  bool alive = false;
};

#endif
