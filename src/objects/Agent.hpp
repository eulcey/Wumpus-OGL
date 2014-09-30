#ifndef AGENT_HPP
#define AGENT_HPP

#include "GameObject.hpp"

class AgentAI;


class Agent: public GameObject
{
public:
  Agent(float xPos, float zPos);
  ~Agent();

  virtual bool release();
  virtual bool init(std::string model, std::string texture, std::string shader);

  // get scenegraph to render, if vision is switched
  TransformNode* getAgentVision();

private:

  // own scenegraph for known information?
  TransformNode *root;

};
#endif
