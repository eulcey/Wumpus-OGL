#ifndef MODELNODE_HPP
#define MODELNODE_HPP

#include "SceneNode.hpp"
#include "MathCore.hpp"
#include <vector>

class RenderContext;

class ModelNode: public SceneNode
{
public:
  ModelNode(std::string name, std::string filename): SceneNode(name) { init(filename); }
  ~ModelNode() {}

  bool accept(NodeVisitor& visitor);
  void update();

  //  bool render(RenderContext& context);
  std::vector<matc::Vector3> getVertices() const { return this->vertices;}
  std::vector<matc::Vector2> getUVs() const { return this->uv; }
  std::vector<matc::Vector3> getNormals() const { return this->normals; }
  /*
  std::vector<float> getVertices() const { return this->vertices; }
  std::vector<float> getUV() const { return this->uv; }
  std::vector<float> getNormal() const { return this->normals; }

  std::vector<unsigned int> getVertexIndices()  const { return this->vIndices; }
  std::vector<unsigned int> getUVIndices() const { return this->uvIndices; }
  std::vector<unsigned int> getNormalIndices() const {return this->normalIndices; }
  */
  int getTrianglesCount() const { return this->vertices.size()/3; }

private:
  bool init(std::string filename);

  std::vector<matc::Vector3> vertices;
  std::vector<matc::Vector2> uv;
  std::vector<matc::Vector3> normals;
  /*
  std::vector<float> vertices;
  std::vector<float> uv;
  std::vector<float> normals;

  std::vector<unsigned int> vIndices;
  std::vector<unsigned int> uvIndices;
  std::vector<unsigned int> normalIndices;
  */
};

#endif
