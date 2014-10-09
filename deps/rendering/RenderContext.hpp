#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP

#include "MathCore.hpp"

class ModelNode;

struct Light
{
  matc::Vector3 color;
  float ambientIntensity;
  matc::Vector3 direction;
  float diffuseIntensity;
};

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
  void setLight(Light light) { this->light = light; }
  void setCameraPosition(matc::Vector3 pos) { this->cameraPosition = pos; }

  
  matc::Matrix4x4 getProjection() const { return projectionTransformation; }
  matc::Matrix4x4 getView() const { return viewTransformation; }
  matc::Matrix4x4 getModel() const { return modelTransformation; }
  std::string getTexture() const { return texture; }
  std::string getShader() const { return shader; }
  Light getLight() const { return light; }
  matc::Vector3 getCameraPosition() const { return cameraPosition; }

  float specularPower = 0.0f;
  float specularIntensity = 0.0f;
private:
  Light light;
  std::string texture;
  std::string shader;
  matc::Matrix4x4 projectionTransformation;
  matc::Matrix4x4 viewTransformation;
  matc::Matrix4x4 modelTransformation = matc::Matrix4x4();
  matc::Vector3 cameraPosition;
};

#endif
