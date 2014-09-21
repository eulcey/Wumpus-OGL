#include "ModelNode.hpp"
#include "NodeVisitor.hpp"
#include "MathCore.hpp"
#include "objloader.hpp"

#include <cstddef>

bool ModelNode::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}

void ModelNode::update()
{
  //  SceneNode::update();
}

bool ModelNode::init(std::string filename)
{
  std::vector<matc::Vector3> temp_vertices;
  std::vector<matc::Vector2> temp_uvs;
  std::vector<matc::Vector3> temp_normals;

  bool res = loadOBJ(filename.c_str(), vertices, uv, normals);
  if(!res) {
    std::cerr << "Error loading file: " << filename << std::endl;
    return false;
  }

  matc::Vector3 firstV = vertices[0];

  std::cout << "first vector: " << firstV << std::endl;
  std::cout << "size: " << sizeof(matc::Vector3) << std::endl;
  std::cout << "offset x: " << offsetof(matc::Vector3, x) << std::endl;
  std::cout << "offset y: " << offsetof(matc::Vector3, y) << std::endl;
  std::cout << "offset z: " << offsetof(matc::Vector3, z) << std::endl;

  return true;
}

/*
bool ModelNode::render(RenderContext& context)
{
  context.render(*this);
}
*/
