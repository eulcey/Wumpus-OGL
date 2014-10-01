#include "AgentLogic.hpp"

#include <cstdlib>

AgentLogic::AgentLogic()
{
  /*
  for(int w = 0; w < width; w++) {
    std::vector<Room*> column;
    for(int h = 0; h < height; h++) {
      column.push_back(new Room);
    }
    rooms.push_back(column);
  }
  */
}

AgentLogic::~AgentLogic()
{
  /*
  for(std::size_t w = 0; w < rooms.size(); w++) {
    std::vector<Room*> column = rooms[w];
    for(std::size_t h = 0; h < column.size(); h++) {
      delete column[h];
    }
    column.clear();
  }
  rooms.clear();
  */
}


void AgentLogic::inputNewSenses(const std::vector<Senses> &newSenses)
{
  for (u_int i = 0; i < newSenses.size(); i++) {
    std::cout << newSenses[i] << " ";
    if(newSenses[i] == Breeze)
      std::cout << "Breeze ";
    if(newSenses[i] == Stench)
      std::cout << "Stench ";
    if(newSenses[i] == Glitter)
      std::cout << "Glitter ";
  }
  std::cout << std::endl;
}

Action AgentLogic::getNextAction()
{
  int action = rand()%3+1;
  return static_cast<Action>(action);
}

void AgentLogic::actionSucceeded(Action action, bool wasSuccessful)
{
  
}
