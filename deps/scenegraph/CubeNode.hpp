#ifndef CUBENODE_HPP
#define CUBENODE_HPP

#include <vector>
#include "MathCore.hpp"
#include "SceneNode.hpp"

class CubeNode: public SceneNode
{
public:
  CubeNode(const std::string name): SceneNode(name) { init(); }
  ~CubeNode() {}

  bool accept(NodeVisitor& visitor);
  void update();
  
  //void setVertices(const std::vector<matc::Vector4> &vertices);
   void setVertices(const std::vector<float> &vertices);
  void setColor(const std::vector<matc::Vector4> &colors);
  //void setColor(const std::vector<float> &colors);
  void setElements(const std::vector<unsigned int> &elements);

  //std::vector<matc::Vector4> getVertices() const { return this->vertices; }
  std::vector<float> getVertices() const { return this->vertices; }
  std::vector<matc::Vector4> getColors() const { return this->colors; }
  //std::vector<float> getColors() const { return this->colors; }
  std::vector<unsigned int> getElements() const { return this->elements;}

private:
  void init();
  //std::vector<matc::Vector4> vertices;
  std::vector<float> vertices;
  std::vector<matc::Vector4> colors;
  //std::vector<float> colors;
  std::vector<unsigned int> elements;
};

#endif
