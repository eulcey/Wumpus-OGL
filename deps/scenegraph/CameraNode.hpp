#ifndef CAMERANODE_HPP
#define CAMERANODE_HPP

#include "SceneNode.hpp"
#include "MathCore.hpp"

class CameraNode: public SceneNode
{
public:
  CameraNode(std::string name, float fov, float aspectRatio, float nearPlane, float farPlane): SceneNode(name), fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane) {}
  ~CameraNode() {}
  
  virtual bool accept(NodeVisitor& visitor);

  matc::Matrix4x4 getProjection();

  inline bool isActive() const { return active; }

  void setActive() { this->active = true; }
  void setInactive() { this->active = false; }
  
private:
  bool active = true;
  float fov;
  float aspectRatio;
  float nearPlane;
  float farPlane;
};

#endif
