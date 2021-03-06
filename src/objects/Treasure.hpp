#ifndef TREASURE_HPP
#define TREASURE_HPP

#include "GameObject.hpp"

class TransformNode;
class MaterialNode;
class ModelNode;

class Treasure: public GameObject
{
public:
  Treasure(float xPos, float yPos);
  ~Treasure();

  virtual void setPosition(float xPos, float yPos);
  virtual bool release();

};
#endif
