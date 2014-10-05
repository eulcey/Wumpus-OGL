#ifndef LEVELLOGIC_HPP
#define LEVELLOGIC_HPP

#include "jsoncpp.h"
#include "MathCore.hpp"
#include "AgentLogic.hpp"

#include <vector>

const int TREASURE_POINTS = 1000;
const int DEATH_POINTS = -1000;
const int STEP_POINTS = -10;
const int SHOOT_POINTS = -100;
const int WUMPUS_POINTS = 1000;

class LevelLogic
{
public:
  LevelLogic(Json::Value &root);
  ~LevelLogic() {}

  //  bool moveAgentTo(const matc::Vector2i &newPos);
  matc::Vector2i getAgentPos() { return agentPos; }
  bool isOnTreasure() { return agentPos == treasurePos && !treasureFound; }
  bool isOnWumpus() { return agentPos == wumpusPos && wumpusAlive; }
  bool isOnExit() { return agentPos == exitPos; }
  bool isOnPit() {
    for (unsigned int i = 0; i < pitsPos.size(); i++) {
      if (pitsPos[i] == agentPos) return true;
    }
    return false;
  }

  std::vector<Senses> getSensorData();
  bool isActionPossible(Action nextAction);

  int getPoints() { return agentPoints; }
  
private:
  matc::Vector2i agentDir = matc::Vector2i(0,1);
  matc::Vector2i worldSize;
  matc::Vector2i agentPos;
  matc::Vector2i exitPos;
  matc::Vector2i wumpusPos;
  matc::Vector2i treasurePos;
  std::vector<matc::Vector2i> pitsPos;

  bool treasureFound = false;
  bool wumpusAlive = true;
  bool agentAlive = true;
  bool arrowAvaible = true;

  int agentPoints = 0;

  bool testDeath();
};

#endif
