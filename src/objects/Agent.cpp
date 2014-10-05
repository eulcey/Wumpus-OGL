#include "Agent.hpp"

#include "TransformNode.hpp"
#include "MaterialNode.hpp"
#include "ModelNode.hpp"
#include "Level.hpp"

Agent::Agent(float xPos, float zPos): GameObject("Agent", xPos, zPos)
{
  init("../assets/agent.obj", "agent", "phongShader");//"shadowShader");//
}

bool Agent::init(std::string model, std::string texture, std::string shader)
{
  return GameObject::init(model, texture, shader);
}

TransformNode* Agent::getAgentVision()
{
  return root;
}

bool Agent::release()
{
  return true;
}

Agent::~Agent()
{
  //  root->release();
  // border.clear();
}
