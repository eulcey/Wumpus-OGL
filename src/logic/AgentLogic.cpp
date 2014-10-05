#include "AgentLogic.hpp"

#include <cstdlib>

using namespace matc;

AgentLogic::AgentLogic()
{
  //  startRoom = new RoomNode();
  //  startRoom->position = Vector2i(pos);
}

AgentLogic::~AgentLogic()
{
}


void AgentLogic::inputNewSenses(const std::set<Senses> &newSenses)
{
  Vector2i position = Vector2i(pos);
  completed.insert(position);
  saveRooms.erase(position);
  auto pGlitter = newSenses.find(Glitter);
  if(pGlitter != newSenses.end()) {
    treasureRoom = new Vector2i(pos);
  }
  auto pBreeze = newSenses.find(Breeze);
  std::set<Vector2i> save;/* = {Vector2i(position) + Vector2i(0,1),
				   Vector2i(position) + Vector2i(0,-1),
				   Vector2i(position) + Vector2i(1,0),
				   Vector2i(position) + Vector2i(-1,0) };
			  */
  Vector2i north = position + Vector2i(0,1);
  Vector2i south = position + Vector2i(0,-1);
  Vector2i east = position + Vector2i(1,0);
  Vector2i west = position + Vector2i(-1,0);
  if(north.x >= 0 && north.y >= 0) save.insert(north);
  if(south.x >= 0 && south.y >= 0) save.insert(south);
  if(east.x >= 0 && east.y >= 0) save.insert(east);
  if(west.x >= 0 && west.y >= 0) save.insert(west);
  
  
  auto pStench = newSenses.find(Stench);
  if(pStench != newSenses.end()) {
    if(wumpusGuesses.size() != 0) {
      std::set<Vector2i> newWumpus;
      for(auto itWumpus = wumpusGuesses.begin(); itWumpus != wumpusGuesses.end(); itWumpus++) {
	Vector2i toFind = Vector2i(*itWumpus);
	auto pFound = save.find(toFind);
	if(pFound != save.end()) {
	  std::cout << "Wumpus idea smaller: " << toFind << std::endl;
	  newWumpus.insert(toFind);
	}
      }
      wumpusGuesses = newWumpus;
    } else {
      wumpusGuesses.insert(save.begin(), save.end());
      // save.clear();
    }
  } else {
    for(auto itSave = save.begin(); itSave != save.end(); itSave++) {
      Vector2i noWumpus = Vector2i(*itSave);
      wumpusGuesses.erase(noWumpus);
    }
  }
  for(auto itVisited = completed.begin(); itVisited != completed.end(); itVisited++) {
    auto itSave = save.find(*itVisited);
    if(itSave != save.end()) {
      // Vector2i already = Vector2i(*itSave);
      save.erase(itSave);
    }
  }
  for(auto itSaveRooms = saveRooms.begin(); itSaveRooms != saveRooms.end(); itSaveRooms++) {
    auto itSave = save.find(*itSaveRooms);
    if(itSave != save.end()) {
      //Vector2i already = Vector2i(*itSave);
      save.erase(itSave);
    }
  }
  
  if(pBreeze != newSenses.end()) {
    pitGuesses.push_back(std::set<Vector2i>(save.begin(), save.end()));
    save.clear();
  } else {
    for(size_t i = 0; i < pitGuesses.size(); i++) {
      for(auto itSave = save.begin(); itSave != save.end(); itSave++) {
	pitGuesses[i].erase(*itSave);
      }
    }
  }
  Vector2i ex = Vector2i(*save.begin());
  
  //std::cout << "inserted in saveRooms for example: " << ex << std::endl;
  saveRooms.insert(save.begin(), save.end());
  
  std::cout << "SaveRooms:" << std::endl;
  for(auto it = saveRooms.begin(); it != saveRooms.end(); it++) {
    Vector2i res = Vector2i(*it);
    std::cout << res << std::endl;
  }
  std::cout << std::endl;
}

// only shoots at wumpus if neighboured
Action AgentLogic::getNextAction()
{
  // if on Treasure
  Encounter thisField = testPosition(pos);
  //std::cout << "Guess for pos" << pos << " was " << thisField << std::endl;
  if(thisField == TreasureEncounter && !hasTreasure) {
    return Grab;
  }
  Encounter nextField = testPosition(pos + dir);
  Vector2i next = pos+dir;
  std::cout << "nextField " << next << " was = " << nextField << std::endl;
  if(nextField == WumpusEncounter && hasArrow) {
    return Shoot;
  }
  if(nextField == FreeEncounter && lastTimeSuccesful[Forward]) {
    //    std::cout << "yeah forward!!!" << std::endl;
    return Forward;
  }
  //return TurnRight;
  int action = rand()%2;
  std::cout << "special action " << action << std::endl;
  return static_cast<Action>(action);
}

void AgentLogic::actionSucceeded(Action action, bool wasSuccessfull)
{
  if(wasSuccessfull) {
    lastTimeSuccesful = {true, true, true, true, true, true};
    int conversion;
    int newX;
    int newY;
    Vector2i lastWumpusPos;
    switch(action) {
    case Forward:
      pos = pos + dir;
      break;
    case TurnLeft:
      conversion = 1;
      if (dir.x == 0) conversion = -1;
      newX = dir.y * conversion;
      newY = dir.x * conversion;
      dir = Vector2i(newX, newY);
      std::cout << "TurnLeft to " << dir << std::endl;
      break;
    case TurnRight:
      conversion = 1;
      if (dir.x != 0) conversion = -1;
      newX = dir.y * conversion;
      newY = dir.x * conversion;
      dir = Vector2i(newX, newY);
      std::cout << "TurnRight to " << dir << std::endl;
      break;
    case Grab:
      hasTreasure = true;
      delete treasureRoom;
      treasureRoom = 0;
      break;
    case Shoot:
      lastWumpusPos = pos + dir;
      saveRooms.insert(lastWumpusPos);
      wumpusGuesses.clear();
      hasArrow = false;
      break;
    default:
      std::cout << "Action success not yet implemented" << std::endl;
      break;
    }
  } else { // action was NOT successful
    switch(action) {
    case Forward:
      lastTimeSuccesful[Forward] = false;
      break;
    case Shoot:
      hasArrow = false;
      break;
    default:
      std::cout << "Action failure not yet implemented" << std::endl;
      break;
    }
  }
}

Encounter AgentLogic::testPosition(Vector2i room)
{
  // std::cout << "testing pos: " << room << std::endl;

  Encounter encounter = UnknownEncounter;
  auto itSaveRooms = saveRooms.find(room);
  if(itSaveRooms != saveRooms.end()) {
    encounter = FreeEncounter;
    auto itWumpus = wumpusGuesses.find(room);
    if(itWumpus != wumpusGuesses.end()) {
      encounter = UnknownEncounter;
    }
  }
  auto itVisited = completed.find(room);
  if(itVisited != completed.end()) {
    encounter = FreeEncounter;
  }

  if(treasureRoom != 0) {
    if(*treasureRoom == room) {
      encounter = TreasureEncounter;
    }
  }

  if(wumpusGuesses.size() == 1) {
    auto wumpusPos = *wumpusGuesses.begin();
    if(room == wumpusPos) {
      encounter = WumpusEncounter;
    }
  }

  return encounter;
}

/*
void AgentLogic::insertSaveToGraph(Vector2i position, std::set<Vector2i> save)
{
  RoomNode *actualRoom = startRoom;

}
*/

std::vector<matc::Vector2i> AgentLogic::getPathToSaveRoom(matc::Vector2i targetRoom)
{
  std::vector<matc::Vector2i> path;
  matc::Vector2i room;

  return path;
}

std::vector<Vector2i> AgentLogic:: getPathRec(std::vector<Vector2i> oldPath,
					      Vector2i target)
{ 
  if(oldPath.back() == target) {
    return oldPath;
  } else {
    std::vector<Vector2i> neighbours = getNeighbours(oldPath.back());
    if(neighbours.size() == 0) {
      return std::vector<matc::Vector2i>();
    }
    return std::vector<matc::Vector2i>();
  }
}

std::vector<Vector2i> AgentLogic::getNeighbours(Vector2i room)
{
  std::vector<matc::Vector2i> neighbours;
  std::set<matc::Vector2i> test = {Vector2i(room) + Vector2i(0,1),
				   Vector2i(room) + Vector2i(0,-1),
				   Vector2i(room) + Vector2i(1,0),
				   Vector2i(room) + Vector2i(-1,0) };
  for(auto itTest = test.begin(); itTest != test.end(); itTest++) {
    Vector2i toFind = Vector2i(*itTest);
    auto found = saveRooms.find(toFind);
    if(found != saveRooms.end()) {
      neighbours.push_back(toFind);
    }
  }
  return neighbours;
}
