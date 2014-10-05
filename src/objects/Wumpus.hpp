#ifndef WUMPUS_HPP
#define WUMPUS_HPP

class SceneNode;
class TransformNode;
class MaterialNode;
class ModelNode;

class Wumpus
{
public:
  Wumpus();
  ~Wumpus();

  bool link(SceneNode &node);
  bool unlink(SceneNode &node);
  bool setPosition(float xpos, float zpos);
private:
  TransformNode *wumpusPosition;
  TransformNode *wumpusScale;
  MaterialNode *wumpusMaterial;
  ModelNode *wumpus;
};

#endif
