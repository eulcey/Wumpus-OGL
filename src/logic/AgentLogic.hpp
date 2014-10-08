#ifndef AGENTLOGIC_HPP
#define AGENTLOGIC_HPP

#include <vector>
#include <stack>
#include <set>
#include <map>
#include <queue>
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

enum Encounter {
  WumpusEncounter,
  TreasureEncounter,
  //PitEncounter,
  FreeEncounter,
  UnknownEncounter
};

struct RoomNode 
{
  matc::Vector2i position;
  std::vector<RoomNode*> children;
};

struct CmpVector2i
{
  bool operator()( matc::Vector2i const& lhs, matc::Vector2i const& rhs ) const
  {
    return lhs < rhs;
  }
};

class AgentLogic
{
public:
  AgentLogic();
  ~AgentLogic();

  void inputNewSenses(const std::set<Senses> &newSenses);
  /* evaluates experience and returns action
   * if treasure is found and wumpus slain, heads back to exit, then Action::Climb
   */
  Action getNextAction();
  void actionSucceeded(Action action, bool wasSuccessful);

  std::vector<matc::Vector2i> getSaveRooms();
  std::vector<matc::Vector2i> getStenchPositions();
  std::vector<matc::Vector2i> getBreezePositions();
  matc::Vector2i getTreasurePos();

private:
  // temp saving for visited places
  std::set<matc::Vector2i> completed;

  std::set<matc::Vector2i> saveRooms;
  std::set<matc::Vector2i> saveWumpusRooms;

  // rooms which are outside the playing field
  std::set<matc::Vector2i> borderRooms;

  // Agent assumes starting point is (1,1)
  matc::Vector2i pos = matc::Vector2i(0,0);
  // Agent assumes starting direction is north
  matc::Vector2i dir = matc::Vector2i(0,1);

  //  void insertSaveToGraph(matc::Vector2i position, std::set<matc::Vector2i> save);
  Encounter testPosition(matc::Vector2i room);

  Action goToNeighbourRoom(matc::Vector2i targetRoom);

  std::map<matc::Vector2i, matc::Vector2i> getPathFromTo(const matc::Vector2i &start, const matc::Vector2i &goal);

  matc::Vector2i getNextToSaveRoom(const matc::Vector2i &targetRoom);
  //  std::vector<matc::Vector2i> getPathRec(std::vector<matc::Vector2i> oldPath,
  //					 matc::Vector2i target);

  std::set<matc::Vector2i> getNeighbours(matc::Vector2i room);

  std::vector<std::set<matc::Vector2i>> pitGuesses;
  std::set<matc::Vector2i> wumpusGuesses;

  matc::Vector2i *treasureRoom = 0;

  //int maxX = 0;
  //int maxY = 0;

  bool hasArrow = true;
  bool hasTreasure = false;
  bool wumpusDead = false;
  
  // was last time this action successful
  std::vector<bool> lastTimeSuccesful = {true, true, true, true, true, true};
};

#endif
