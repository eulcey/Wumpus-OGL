#ifndef AGENT_HPP
#define AGENT_HPP

#include "GameObject.hpp"

struct place
{
  int x;
  int y;
};

class Agent: public GameObject
{
public:
  Agent(float xPos, float zPos): GameObject("Agent", xPos, zPos)
  { init("../assets/agent.obj", "agent", "phongShader");}
  ~Agent();

  virtual bool release();

  // gets data from current field, maybe invoke getSensorData(x, y) from Scene/Level
  // maybe: adds safe fields to "border"
  void sense();

  // act appropriate to KI and Sensordata
  //   1. collects treasure if on field
  //   2. shoot wumpus if avaible
  //   3. goes to next safe field
  // order not sure
  void act();

  // get scenegraph to render, if vision is switched
  TransformNode getAgentVision();

private:
  // Sensordata
  
  // Border: places not yet visited, but safe to visit
  std::vector<place> border;

  // own scenegraph for known information?
  TransformNode *root;
};
#endif
