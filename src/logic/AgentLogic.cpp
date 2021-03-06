#include "AgentLogic.hpp"

#include <cstdlib>
#include <queue>

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

  std::set<Vector2i> save;
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
    stenchRooms.insert(Vector2i(pos));
    if(wumpusGuesses.size() != 0) {
      std::set<Vector2i> newWumpus;
      for(auto itWumpus = wumpusGuesses.begin(); itWumpus != wumpusGuesses.end(); itWumpus++) {
	Vector2i toFind = Vector2i(*itWumpus);
	auto pFound = save.find(toFind);
	if(pFound != save.end()) {
	  auto pSaveWumpus = saveWumpusRooms.find(toFind);
	  if(pSaveWumpus == saveWumpusRooms.end()) {
	    //    std::cout << "Wumpus idea smaller: " << toFind << std::endl;
	    newWumpus.insert(toFind);
	  }
	}
      }
      wumpusGuesses = newWumpus;
    } else {
      for(auto itSave = save.begin(); itSave != save.end(); itSave++) {
	Vector2i toFind = Vector2i(*itSave);
	auto pSaveWumpus = saveWumpusRooms.find(toFind);
	if(pSaveWumpus == saveWumpusRooms.end()) {
	  wumpusGuesses.insert(toFind);
	}
      }
      // save.clear();
    }
  } else {
    for(auto itSave = save.begin(); itSave != save.end(); itSave++) {
      Vector2i noWumpus = Vector2i(*itSave);
      wumpusGuesses.erase(noWumpus);
      saveWumpusRooms.insert(noWumpus);
    }
  }
  for(auto itVisited = completed.begin(); itVisited != completed.end(); itVisited++) {
    auto itSave = save.find(*itVisited);
    if(itSave != save.end()) {
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
 
  auto pBreeze = newSenses.find(Breeze);
  if(pBreeze != newSenses.end()) {
    breezeRooms.insert(Vector2i(pos));
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
  /*
  std::cout << "SaveRooms:" << std::endl;
  for(auto it = saveRooms.begin(); it != saveRooms.end(); it++) {
    Vector2i res = Vector2i(*it);
    auto itBorder = borderRooms.find(res);
    if(itBorder == borderRooms.end()) {
      std::cout << res << std::endl;
    }
  }
  std::cout << std::endl;
  */
}

// only shoots at wumpus if neighboured
Action AgentLogic::getNextAction()
{
  
  if((hasTreasure && wumpusDead) || saveRooms.size() <= 0) { // if level completed or no save spots, go to exit
    if(pos == Vector2i(0,0)) {
      return Leave;
    } else {
      auto nextRoom = getNextToSaveRoom(Vector2i(0,0));
      if (nextRoom == pos) {
	return goToNeighbourRoom(Vector2i(0,0));
      } else {
	return goToNeighbourRoom(nextRoom);
      }
    }
  }
  
  // if on Treasure
  Encounter thisField = testPosition(pos);
  if(thisField == TreasureEncounter && !hasTreasure) {
    return Grab;
  }
  
  Encounter nextField = testPosition(pos + dir);
  Vector2i next = pos+dir;
  if(nextField == WumpusEncounter && hasArrow) {
    return Shoot;
  }

  // find save room without wumpus guess
  Vector2i nextSaveRoom;
  for(auto itSave = saveRooms.begin(); itSave != saveRooms.end(); itSave++) {
    auto wumpusGuess = wumpusGuesses.find(*itSave);
    if(wumpusGuess == wumpusGuesses.end()){
      auto itBorder = borderRooms.find(*itSave);
      if(itBorder == borderRooms.end()) {
	nextSaveRoom = *itSave;
      }
    }
  }
  // else explore unvisited save rooms
  
  auto nextRoom = getNextToSaveRoom(nextSaveRoom);
  if (nextRoom == pos) {
    return goToNeighbourRoom(nextSaveRoom);
  } else {
    return goToNeighbourRoom(nextRoom);
  }
}

void AgentLogic::actionSucceeded(Action action, bool wasSuccessfull)
{
  if(wasSuccessfull) {
    lastTimeSuccesful = {true, true, true, true, true, true};
    int conversion;
    int newX;
    int newY;
    Vector2i lastTestedPos;
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
      break;
    case TurnRight:
      conversion = 1;
      if (dir.x != 0) conversion = -1;
      newX = dir.y * conversion;
      newY = dir.x * conversion;
      dir = Vector2i(newX, newY);
      break;
    case Grab:
      hasTreasure = true;
      delete treasureRoom;
      treasureRoom = 0;
      break;
    case Shoot:
      lastTestedPos = pos + dir;
      saveRooms.insert(lastTestedPos);
      wumpusGuesses.clear();
      hasArrow = false;
      wumpusDead = true;
      break;
    case Leave:
      break;
    default:
      std::cout << "Action success not yet implemented" << std::endl;
      break;
    }
  } else { // action was NOT successful
    Vector2i lastTestedPos;
    switch(action) {
    case Forward:
      lastTimeSuccesful[Forward] = false;
      lastTestedPos = pos + dir;
      borderRooms.insert(lastTestedPos);
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
  Encounter encounter = UnknownEncounter;
  
  auto itSaveRooms = saveRooms.find(room);
  if(itSaveRooms != saveRooms.end()) {
    encounter = FreeEncounter;
    auto itWumpus = wumpusGuesses.find(room);
    if(itWumpus != wumpusGuesses.end()) {
      encounter = UnknownEncounter;
    }
  }
  
  auto itOutside = borderRooms.find(room);
  if(itOutside != borderRooms.end()) {
    encounter = UnknownEncounter;
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

Action AgentLogic::goToNeighbourRoom(Vector2i targetRoom)
{
  if(pos + dir == targetRoom) {
    return Forward;
  } else {
    return TurnRight;
  }
}

std::map<Vector2i,Vector2i> AgentLogic::getPathFromTo(const Vector2i &start, const Vector2i &goal)
{
  std::map<Vector2i, Vector2i> foundBy;
  std::set<Vector2i> visited;
  std::queue<Vector2i> border;
  border.push(start);
  visited.insert(start);
  while(!border.empty()) {
    Vector2i node = border.front();
    border.pop();
    if(node == goal) {
      return foundBy;
    }
    auto children = getNeighbours(node);
    for(auto itChild = children.begin(); itChild != children.end(); itChild++) {
      auto child = *itChild;
      auto itVisited = visited.find(child);
      if(itVisited == visited.end()) {
	foundBy[child] = node;
	border.push(child);
	visited.insert(child);
      }
    }
  }
  return foundBy;
}

matc::Vector2i AgentLogic::getNextToSaveRoom(const matc::Vector2i &targetRoom)
{
  //std::vector<matc::Vector2i> path;

  auto foundBy = getPathFromTo(targetRoom, pos);

  return foundBy[pos];
}

std::set<Vector2i> AgentLogic::getNeighbours(Vector2i room)
{
  std::set<matc::Vector2i> neighbours;
  std::set<matc::Vector2i> test = {Vector2i(room) + Vector2i(0,1),
				   Vector2i(room) + Vector2i(0,-1),
				   Vector2i(room) + Vector2i(1,0),
				   Vector2i(room) + Vector2i(-1,0) };
  for(auto itTest = test.begin(); itTest != test.end(); itTest++) {
    Vector2i toFind = Vector2i(*itTest);
    auto wumpusGuess = wumpusGuesses.find(toFind);
    if(wumpusGuess == wumpusGuesses.end()) {
      auto found = saveRooms.find(toFind);
      if(found != saveRooms.end()) {
	neighbours.insert(toFind);
      }
    }
    auto foundComplete = completed.find(toFind);
    if(foundComplete != completed.end()) {
      neighbours.insert(toFind);
    }
  }
  return neighbours;
}

std::set<Vector2i> AgentLogic::getSaveRooms()
{
  std::set<Vector2i> save;
  for(auto it = saveRooms.begin(); it != saveRooms.end(); it++) {
    Vector2i res = Vector2i(*it);
    auto itBorder = borderRooms.find(res);
    if(itBorder == borderRooms.end()) {
      save.insert(res);
    }
  }

  for(auto it = wumpusGuesses.begin(); it != wumpusGuesses.end(); it++) {
    Vector2i res = Vector2i(*it);
    save.erase(res);
  } 

  for(auto it = completed.begin(); it != completed.end(); it++) {
    Vector2i res = Vector2i(*it);
    save.insert(res);
  }

  return save;
}

std::set<Vector2i> AgentLogic::getStenchPositions()
{
  std::set<Vector2i> stenches;
  
  if(!wumpusDead) { // only stenches if Wumpus still alive
    stenches.insert(stenchRooms.begin(), stenchRooms.end());
  }
  
  return stenches;
}

std::set<Vector2i> AgentLogic::getBreezePositions()
{
  std::set<Vector2i> breezes;

  breezes.insert(breezeRooms.begin(), breezeRooms.end());

  return breezes;
}

Vector2i AgentLogic::getTreasurePos()
{
  if(treasureRoom != 0) {
    return Vector2i(treasureRoom->x, treasureRoom->y);
  } else {
    return Vector2i(-1, -1);
  }
}

