#include "CubeNode.hpp"
#include "NodeVisitor.hpp"

using namespace matc;

bool CubeNode::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
  return true;
}

//void CubeNode::setVertices(const std::vector<Vector4> &vertices)
void CubeNode::setVertices(const std::vector<float> &vertices)
{
  this->vertices = vertices;
}

void CubeNode::setColor(const std::vector<Vector4> &colors)
//void CubeNode::setColor(const std::vector<float> &colors)
{
  this->colors = colors;
}

void CubeNode::setElements(const std::vector<unsigned int> &elements)
{
  this->elements = elements;
}

void CubeNode::init()
{
  
  vertices = /*{
    Vector4(-0.5f, -0.5f, 0.5f, 1),
    Vector4(-0.5f, 0.5f, 0.5f, 1),
    Vector4(0.5f, 0.5f, 0.5f, 1),
    Vector4(0.5f, -0.5f, 0.5f, 1),
    Vector4(-0.5f, -0.5f, -0.5f, 1),
    Vector4(-0.5f, 0.5f, -0.5f, 1),
    Vector4(0.5f, 0.5f, -0.5f, 1),
    Vector4(0.5f, -0.5f, -0.5f, 1)
    };
	     */
  vertices = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, 
    1.0f,-1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
  };
  
  colors = /*{
    Vector4(1, 0, 0, 1),
    Vector4(1, 0, 0, 1),
    Vector4(1, 0, 0, 1),
    Vector4(1, 0, 0, 1),
    Vector4(1, 0, 0, 1),
    Vector4(1, 0, 0, 1),
    Vector4(1, 0, 0, 1),
    Vector4(1, 0, 0, 1)
    }; 
  {
    Vector4(1, 1, 0, 1),
    Vector4(1, 1, 0, 1),
    Vector4(0, 1, 1, 1),
    Vector4(0, 1, 1, 1),
    Vector4(1, 1, 0, 1),
    Vector4(1, 1, 0, 1),
    Vector4(0, 1, 1, 1),
    Vector4(0, 1, 1, 1)
    };*/
    {
    Vector4(0, 0, 1, 1),
    Vector4(1, 0, 0, 1),
    Vector4(0, 1, 0, 1),
    Vector4(1, 1, 0, 1),
    Vector4(1, 1, 1, 1),
    Vector4(1, 0, 0, 1),
    Vector4(1, 0, 1, 1),
    Vector4(0, 0, 1, 1)
    };
    
    
  elements = {
    0, 1, 2,  3, 0, 4,  
    5, 0, 6,  3, 6, 0, 
    0, 2, 4,  5, 1, 0,
    2, 1, 5,  7, 5, 3, 
    6, 7, 5,  7, 3, 4, 
    7, 4, 2,  7, 2, 5 
  };
  /*
  elements = {
    Vector3(0, 2, 1), Vector3(0, 3, 2),
    Vector3(4, 3, 0), Vector3(4, 7, 3),
    Vector3(4, 1, 5), Vector3(4, 0, 1),
    Vector3(3, 6, 2), Vector3(3, 7, 6),
    Vector3(1, 6, 5), Vector3(1, 2, 6),
    Vector3(7, 5, 6), Vector3(7, 4, 5)
  };
  */
}

void CubeNode::update()
{
}
