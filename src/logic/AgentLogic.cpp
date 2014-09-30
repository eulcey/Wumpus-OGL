#include "AgentLogic.hpp"

AgentLogic::AgentLogic(int width, int height)
{
  for(int w = 0; w < width; w++) {
    std::vector<Room*> column;
    for(int h = 0; h < height; h++) {
      column.push_back(new Room);
    }
    rooms.push_back(column);
  }
}

AgentLogic::~AgentLogic()
{
  for(int w = 0; w < rooms.size(); w++) {
    std::vector<Room*> column = rooms[w];
    for(int h = 0; h < column.size(); h++) {
      delete column[h];
    }
    column.clear();
  }
  rooms.clear();
}
