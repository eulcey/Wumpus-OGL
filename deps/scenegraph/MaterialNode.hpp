#ifndef MATERIALNODE_HPP
#define MATERIALNODE_HPP

#include "SceneNode.hpp"

class NodeVisitor;

class MaterialNode: public SceneNode
{
public:
  MaterialNode(std::string name, std::string texture, std::string vertexShader, std::string fragmentShader):
    SceneNode(name), texturePath(texture), vertexShaderPath(vertexShader), fragmentShaderPath(fragmentShader) {}
  ~MaterialNode() {}

  bool accept(NodeVisitor &visitor);

  std::string getTexture() const { return texturePath; }
  std::string getVertexShader() const { return vertexShaderPath; }
  std::string getFragmentShader() const { return fragmentShaderPath; }
  
private:
  std::string texturePath;
  std::string vertexShaderPath;
  std::string fragmentShaderPath;
};
  
#endif
