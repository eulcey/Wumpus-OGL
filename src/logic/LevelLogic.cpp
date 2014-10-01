#include "LevelLogic.hpp"

using namespace matc;

LevelLogic::LevelLogic(Json::Value &rootValue)
{
  
  std::string levelName = rootValue["LevelName"].asString();
  int width = rootValue["Width"].asInt();
  int height = rootValue["Height"].asInt();
  worldSize = Vector2i(width, height);

  Json::Value wumpusValue = rootValue["Wumpus"];
  Json::Value agentValue = rootValue["Agent"];
  Json::Value treasureValue = rootValue["Treasure"];

  Json::Value pitsValue = rootValue["Pits"];
  for(u_int index = 0; index < pitsValue.size(); index++) {
    int pitX = pitsValue[index]["xpos"].asInt() - 1;
    int pitZ = pitsValue[index]["zpos"].asInt() - 1;
    Vector2i pit(pitX, pitZ);
    pitsPos.push_back(pit);
  }

  /*
   * ==============
   * Agent, Wumpus, Treasure initialize here
   */
  int agentX = agentValue["xpos"].asInt() - 1;
  int agentZ = agentValue["zpos"].asInt() - 1;
  agentPos = Vector2i(agentX, agentZ);
  exitPos = Vector2i(agentX, agentZ);
  
  int wumpusX = wumpusValue["xpos"].asInt()-1;
  int wumpusZ = wumpusValue["zpos"].asInt()-1;
  wumpusPos = Vector2i(wumpusX, wumpusZ);
  
  int treasureX = treasureValue["xpos"].asInt() - 1;
  int treasureZ = treasureValue["zpos"].asInt() - 1;
  treasurePos = Vector2i(treasureX, treasureZ);
}

/*
bool LevelLogic::moveAgentTo(const matc::Vector2i &newPos) {
  if(worldSize.x - newPos.x < 0 || worldSize.y - newPos.y < 0) {
    return false;
  } else {
    agentPos = newPos;
    return true;
  }
}
*/

std::vector<Senses> LevelLogic::getSensorData() {
  std::vector<Senses> senses;
  
  Vector2i north(agentPos.x, agentPos.y+1);
  Vector2i south(agentPos.x, agentPos.y-1);
  Vector2i east(agentPos.x-1, agentPos.y);
  Vector2i west(agentPos.x+1, agentPos.y);
  
  if (wumpusPos == north || wumpusPos == south || wumpusPos == east || wumpusPos == west) {
    senses.push_back(Stench);
  }
  if (treasurePos == agentPos) {
    senses.push_back(Glitter);
  }
  for(u_int i = 0; i < pitsPos.size(); i++) {
    if (pitsPos[i] == north || pitsPos[i] == south || pitsPos[i] == east || pitsPos[i] == west) {
      senses.push_back(Breeze);
      break;
    }
  }
  return senses;
}


bool LevelLogic::isActionPossible(Action nextAction)
{
  if(nextAction == TurnLeft) {
    // turn agent
    int conversion = 1;
    if (agentDir.x == 0) conversion = -1;
    int newX = agentDir.y * conversion;
    int newY = agentDir.x * conversion;
    agentDir = Vector2i(newX, newY);
    //std::cout << "after turn: " << agentDir << std::endl;
    return true;
  }
  else if(nextAction == TurnRight) {
    // turn agent
    int conversion = 1;
    if (agentDir.x != 0) conversion = -1;
    int newX = agentDir.y * conversion;
    int newY = agentDir.x * conversion;
    agentDir = Vector2i(newX, newY);
    //std::cout << "after turn: " << agentDir << std::endl;
    return true;
  }
  else if(nextAction == Grab) {
    if(agentPos == treasurePos && !treasureFound) {
      // grab Treasure
      treasureFound = true;
      agentPoints += TREASURE_POINTS;
      return true;
    } else
      return false;
  }
  else if(nextAction == Forward) {
    int newX = agentPos.x + agentDir.x;
    int newY = agentPos.y + agentDir.y;
    if(newX >= 0 && newX < worldSize.x && newY >= 0 && newY < worldSize.y) {
      agentPos = Vector2i(newX, newY);
      if(testDeath()) {
	agentAlive = false;
	agentPoints += DEATH_POINTS;
      }
      return true;
    }
    return false;
  }
  else if(nextAction == Leave) {
    if(agentPos == exitPos) {
      // leave
      return true;
    } else
      return false;
  }

  std::cout << "LEVELLOGIC: Action: " << nextAction << " not implemented" << std::endl;
  return false;
}

bool LevelLogic::testDeath() {
  if(agentPos == wumpusPos) {
    return true;
  }
  for(u_int i = 0; i < pitsPos.size(); i++) {
    if (agentPos == pitsPos[i]) {
      return true;
    }
  }
  return false;
}
