#ifndef AGENTLOGIC_HPP
#define AGENTLOGIC_HPP

#include <vector>

enum Senses {
  Breeze,
  Stench,
  Glitter
};

enum Action {
  TurnLeft,
  TurnRight,
  Forward,
  Shoot
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
  AgentLogic(int width, int height);
  ~AgentLogic();

   

private:
  std::vector<std::vector<Room*>> rooms;
};

#endif
