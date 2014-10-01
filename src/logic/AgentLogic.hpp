#ifndef AGENTLOGIC_HPP
#define AGENTLOGIC_HPP

#include <vector>
#include "MathCore.hpp"

enum Senses {
  Breeze,
  Stench,
  Glitter
};

enum Action {
  TurnLeft,
  TurnRight,
  Forward,
  Shoot,
  Grab,
  Leave
};

struct Room
{
  int x;
  int y;
  std::vector<Senses> senses;
};

class AgentLogic
{
public:
  AgentLogic();
  ~AgentLogic();

  void inputNewSenses(const std::vector<Senses> &newSenses);
  Action getNextAction();
  void actionSucceeded(Action action, bool wasSuccessful);

private:
  // relative position to starting position
  matc::Vector2i ownRelPos;
  //std::vector<std::vector<Room*>> rooms;

  bool haveArrow = true;
  bool haveTreasure = false;
  
};

#endif
