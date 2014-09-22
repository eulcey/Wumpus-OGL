#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP

#include "MathCore.hpp"

class ModelNode;

class RenderContext
{
public:
  RenderContext(const matc::Matrix4x4& proj, const matc::Matrix4x4& view):
    projectionTransformation(proj), viewTransformation(view) {}
  RenderContext(): RenderContext(matc::Matrix4x4(), matc::Matrix4x4()) {}
  ~RenderContext() {}

  void setProjection(matc::Matrix4x4);
  void setView(matc::Matrix4x4);
  void setModel(matc::Matrix4x4);
  
  void setTexture(std::string texture) { this->texture = texture; }
  void setShader(std::string shader) { this->shader = shader; }
  //void setVertexShader(std::string vertex) { this->vertexShader = vertex; }
  //void setFragmentShader(std::string fragment) { this->fragmentShader = fragment; }
  // bool render(ModelNode& model);
  matc::Matrix4x4 getProjection() const { return projectionTransformation; }
  matc::Matrix4x4 getView() const { return viewTransformation; }
  matc::Matrix4x4 getModel() const { return modelTransformation; }

  std::string getTexture() const { return texture; }
  std::string getShader() const { return shader; }
  //std::string getVertexShader() const { return vertexShader; }
  //std::string getFragmentShader() const { return fragmentShader; }
private:
  std::string texture;
  std::string shader;
  // std::string vertexShader;
  // std::string fragmentShader;
  // shader or program
  matc::Matrix4x4 projectionTransformation;
  matc::Matrix4x4 viewTransformation;
  matc::Matrix4x4 modelTransformation = matc::Matrix4x4();
};

#endif
