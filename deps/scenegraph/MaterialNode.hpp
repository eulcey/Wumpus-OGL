#ifndef MATERIALNODE_HPP
#define MATERIALNODE_HPP

#include "SceneNode.hpp"

class NodeVisitor;

class MaterialNode: public SceneNode
{
public:
  MaterialNode(std::string name, std::string texture, std::string shader):
    SceneNode(name), textureName(texture), shaderName(shader) {} 
  ~MaterialNode() {}

  bool accept(NodeVisitor &visitor);

  std::string getTexture() const { return textureName; }
  std::string getShader() const { return shaderName; }
  
  float specularIntensity = 0.0f;
  int specularPower = 0.0f;
  
private:
  std::string textureName;
  std::string shaderName;
};
  
#endif
