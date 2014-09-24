#ifndef AGENT_HPP
#define AGENT_HPP

#include "GameObject.hpp"

class Agent: public GameObject
{
public:
  Agent(float xPos, float zPos): GameObject(xPos, zPos) {init();}
  ~Agent() {}

  virtual bool init();
  virtual bool release();
};
#endif
