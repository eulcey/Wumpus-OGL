#ifndef CAMERA_HPP
#define CAMERA_HPP

//#include "TransformNode.hpp"
//#include "CameraNode.hpp"
#include "MathCore.hpp"

class SceneNode;
class TransformNode;
class CameraNode;

const double MOUSE_SPEED = 0.00005;
const double MOVE_SPEED = 0.05;
const matc::Vector3 startPosition(10.0, 75.0, 70.0);
const float START_HORIZONTAL_ANGLE = 3.14f*1.12;
const float START_VERTICAL_ANGLE = -3.14f/3;

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
  bool unlink(SceneNode &link);
  bool addSkybox(SceneNode &skybox);
  void onKeyboard(int key, int action);
  void update(float deltaTime);
  void changeView(double xpos, double ypos, float deltaTime);

  bool switchMouseLook();
  bool mouseLookActive() { return mouseLookEnabled; }

  void changeScreen(matc::Vector2i newSize);
  void changeFOV(float newFOV);

  void activate();
  void deactivate();
  void reset();

  TransformNode* getTransform() { return rotation; }
  TransformNode *position;
  
private:
  int width, height;
  CameraNode *camera;
  TransformNode *rotation;
  float horizontalAngle = START_HORIZONTAL_ANGLE;
  float verticalAngle = START_VERTICAL_ANGLE;
  matc::Vector3 direction;
  matc::Vector3 right;

  bool mouseLookEnabled = true;
  bool move_pressed[4] = {false, false, false, false};
};
#endif
