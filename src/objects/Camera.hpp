#ifndef CAMERA_HPP
#define CAMERA_HPP

//#include "TransformNode.hpp"
//#include "CameraNode.hpp"
#include "MathCore.hpp"

class SceneNode;
class TransformNode;
class CameraNode;

const double MOUSE_SPEED = 0.0005;
const double MOVE_SPEED = 0.02;

namespace movement
{
  const int FORWARD = 0;
  const int BACKWARD = 1;
  const int LEFT = 2;
  const int RIGHT = 3;

  const int PRESSED = 0;
}

class Camera
{
public:
Camera(std::string name, int screenWidth, int screenHeight);
~Camera();

  bool link(SceneNode &link);
  bool addSkybox(SceneNode &skybox);
  void onKeyboard(int key, int action);
  void update(float deltaTime);
  void changeView(double xpos, double ypos, float deltaTime);

  void activate();
  void deactivate();
  
private:
  int width, height;
  CameraNode *camera;
  TransformNode *position;
  TransformNode *rotation;
  float horizontalAngle = 3.14f;
  float verticalAngle = 0.0f;
  matc::Vector3 direction;
  matc::Vector3 right;

  bool move_pressed[4] = {false, false, false, false};
};
#endif
