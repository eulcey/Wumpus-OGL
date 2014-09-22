#include "Camera.hpp"

//#include "MathCore.hpp"
#include "TransformNode.hpp"
#include "CameraNode.hpp"

using namespace matc;

Camera::Camera(std::string name, int screenWidth, int screenHeight, double time): width(screenWidth), height(screenHeight), lastTime(time)
{
  Vector3 startPosition(0.0, 2.0, 4.0);
  Matrix4x4 viewMatrix = Matrix4x4();//lookAt(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0)).invert();
  Matrix4x4 posMatrix = translate(Matrix4x4(), startPosition);
  std::string rotationName = name + "-Rotation";
  rotation = new TransformNode(rotationName.c_str(), viewMatrix);
  std::string positionName = name + "-Position";
  position = new TransformNode(positionName.c_str(), posMatrix);
  camera = new CameraNode(name, 45.0f, 1.0f*screenWidth/screenHeight, 0.1f, 100.0f);

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

bool Camera::addSkybox(SceneNode& skybox)
{
  position->addChild(&skybox);
  return true;
}

void Camera::onKeyboard(int move, int action)
{
  move_pressed[move] = action;
  
  // std::cout << "Button " << move << "pressed: " << move_pressed[move] << std::endl;
  //Vector3 direction;
  /*
  if(move == movement::FORWARD) {
    direction = this->direction * -1;
  } else if(move == movement::BACKWARD) {
    direction =  this->direction;
  } else if(move == movement::RIGHT) {
    direction = this->right * -1;
  } else if(move == movement::LEFT){
    direction = this->right;
  } else {
    direction = Vector3(0, 0, 0);
  }
  */
 
}

void Camera::update(double currentTime)
{
  float deltaTime = float(currentTime - lastTime);
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
  Matrix4x4 newPosition = translate(oldPosition, Vector3(posChange.x, 0/*posChange.y*/, posChange.z));
  position->setTransform(newPosition);
}

void Camera::changeView(double xpos, double ypos, double currentTime)
{
  float deltaTime = float(currentTime - lastTime);
  
  horizontalAngle += MOUSE_SPEED * deltaTime * float(width/2 - xpos);
  verticalAngle += MOUSE_SPEED * deltaTime * float(height/2 - ypos);
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

void Camera::activate()
{
  camera->setActive();
}

void Camera::deactivate()
{
  camera->setInactive();
}
