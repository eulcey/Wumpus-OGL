#include "Camera.hpp"

//#include "MathCore.hpp"
#include "TransformNode.hpp"
#include "CameraNode.hpp"

using namespace matc;

Camera::Camera(std::string name, int screenWidth, int screenHeight): width(screenWidth), height(screenHeight)
{
  
  //Matrix4x4 viewMatrix =
    //rotate(rotate(Matrix4x4(), -3.14f/4, Vector3(1.0, 0.0, 0.0)), 3.14f/4, Vector3(0.0, 1.0, 0.0));
    //lookAt(Vector3(0.0, 0.0, 0.0), Vector3(20.0, 0.0, 10.0), Vector3(0.0, 1.0, 0.0)).invert();
  Matrix4x4 posMatrix = translate(Matrix4x4(), startPosition);
  std::string rotationName = name + "-Rotation";
  //rotation = new TransformNode(rotationName.c_str(), Matrix4x4());
  std::string positionName = name + "-Position";
  position = new TransformNode(positionName.c_str(), posMatrix);
  camera = new CameraNode(name, 45.0f, 1.0f*screenWidth/screenHeight, 0.1f, 200.0f);

  
  Vector3 direction(cos(verticalAngle) * sin(horizontalAngle),
		    sin(verticalAngle),
		    cos(verticalAngle) * cos(horizontalAngle)
		    );
  Vector3 right(sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
		);
  Vector3 up = right.cross(direction);

  this->direction = direction;
  this->right = right;
  
  //rotation->setTransform(lookAt(Vector3(0.0, 0.0, 0.0), direction, up).invert());
  rotation = new TransformNode(rotationName.c_str(), lookAt(Vector3(0.0, 0.0, 0.0), direction, up).invert());
  
  rotation->addChild(camera);
  position->addChild(rotation);

}

Camera::~Camera()
{
  camera->release(); position->release(); rotation->release();
}

bool Camera::link(SceneNode& node)
{
  node.addChild(this->position);
  //node.addChild(rotation);
  return true;
}

bool Camera::unlink(SceneNode& link)
{
  return link.removeChild(*this->position);
}

bool Camera::addSkybox(SceneNode& skybox)
{
  position->addChild(&skybox);
  return true;
}

bool Camera::switchMouseLook()
{
  mouseLookEnabled = !mouseLookEnabled;
  return mouseLookEnabled;
}

void Camera::changeScreen(Vector2i newSize)
{
  this->width = newSize.x;
  this->height = newSize.y;
  camera->changeAspectRatio(1.0f*width/height);
}

void Camera::changeFOV(float fov)
{
  camera->changeFOV(fov);
}

void Camera::onKeyboard(int move, int action)
{
  move_pressed[move] = action;
}

void Camera::update(float deltaTime)
{
  //  float deltaTime = float(currentTime - lastTime);
  Vector3 posChange(0, 0, 0);
  if(move_pressed[movement::FORWARD]) {
    posChange = posChange + direction * deltaTime * MOVE_SPEED;
  }
  if(move_pressed[movement::BACKWARD]) {
    posChange = posChange - direction * deltaTime * MOVE_SPEED;
  }
  if(move_pressed[movement::RIGHT]) {
    posChange = posChange + right * deltaTime * MOVE_SPEED;
  }
  if(move_pressed[movement::LEFT]) {
    posChange = posChange - right * deltaTime * MOVE_SPEED;
  }
  //direction = direction.normalize();
  //Vector3 posChange = direction * deltaTime * MOVE_SPEED;
  Matrix4x4 oldPosition = Matrix4x4(position->getTransform());
  float actualYChange = 0;
  if (mouseLookEnabled){ // not actual desired movement if mouselook=false
    if(posChange.y > 0 || oldPosition.asArray()[13] > 23) // no lower than 23, to prevent clipping hud with world
      actualYChange = posChange.y;
  }
  Matrix4x4 newPosition = translate(oldPosition, Vector3(posChange.x, actualYChange, posChange.z));
  position->setTransform(newPosition);
}

void Camera::changeView(double xpos, double ypos, float deltaTime)
{
  if(mouseLookEnabled) {
    horizontalAngle += MOUSE_SPEED * deltaTime * float(width/2 - xpos);
    verticalAngle += MOUSE_SPEED * deltaTime * float(height/2 - ypos);
    if(verticalAngle > 3.14f/3.0f) {
      verticalAngle = 3.14f/3.0f;
    }
    if(verticalAngle < -3.14f/3.0f) {
      verticalAngle = -3.14f/3.0f;
    }
    Vector3 direction(cos(verticalAngle) * sin(horizontalAngle),
		      sin(verticalAngle),
		      cos(verticalAngle) * cos(horizontalAngle)
		      );
    Vector3 right(sin(horizontalAngle - 3.14f/2.0f),
		  0,
		  cos(horizontalAngle - 3.14f/2.0f)
		  );
    Vector3 up = right.cross(direction);

    this->direction = direction;
    this->right = right;

    rotation->setTransform(lookAt(Vector3(0.0, 0.0, 0.0), direction, up).invert());
  }
}

void Camera::activate()
{
  camera->setActive();
}

void Camera::deactivate()
{
  camera->setInactive();
}

void Camera::reset()
{
  Matrix4x4 posMatrix = translate(Matrix4x4(), startPosition);
  position->setTransform(posMatrix);
  horizontalAngle = START_HORIZONTAL_ANGLE;
  verticalAngle = START_VERTICAL_ANGLE;
  
  Vector3 direction(cos(verticalAngle) * sin(horizontalAngle),
		    sin(verticalAngle),
		    cos(verticalAngle) * cos(horizontalAngle)
		    );
  Vector3 right(sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
		);
  Vector3 up = right.cross(direction);

  this->direction = direction;
  this->right = right;
  
  //rotation->setTransform(lookAt(Vector3(0.0, 0.0, 0.0), direction, up).invert());
  rotation->setTransform(lookAt(Vector3(0.0, 0.0, 0.0), direction, up).invert());
}
