#include <iostream>
#include "MathCore.hpp"

#include "Printer.hpp"
#include "Renderer.hpp"
//#include "CubeNode.hpp"
//#include "ModelNode.hpp"
//#include "CameraNode.hpp"
#include "OGLRenderEngine.hpp"
#include "UserInput.hpp"
#include "Scene.hpp"
//#include "Camera.hpp"
//#include "World.hpp"

using namespace matc;
using namespace std;

const int screen_width = 1200;
const int screen_height = 1024;
const float mouseSpeed = 0.0005;
const float speed = 0.01;

//Matrix4x4 computeTranslation(int, double, Vector3&);
//Matrix4x4 computeView(double, double, float&, float&, double, Vector3);

Vector3 globalDirection;
Vector3 globalRight;

int main(void)
{
  
  UserInput user;
  Scene scene(screen_width, screen_height, &user);
  scene.load("../saved_worlds/original_world");

  Camera *camera = scene.getCamera();
  
  float lastTime = glfwGetTime();
  
  OGLRenderEngine engine(screen_width, screen_height, "Wumpuse-OGL", &user);
  
  user.setMouseInputAction(GLFW_MOUSE_BUTTON_LEFT, [] { std::cout << "left mouse button clicked" << std::endl; });
  user.setMouseInputAction(GLFW_MOUSE_BUTTON_RIGHT, [] { std::cout << "right mouse button clicked" << std::endl; });


  user.setKeyboardInputAction(GLFW_KEY_F, [&] (int action) { if(action) scene.switchMouseLook(); });

  /*
  user.setMousePosAction([&] (double xpos, double ypos) {
      camera->changeView(xpos, ypos, float(glfwGetTime()-lastTime));
    });
  */
  user.setKeyboardInputAction(GLFW_KEY_W, [&] (int action) { camera->onKeyboard(movement::FORWARD, action); });
  user.setKeyboardInputAction(GLFW_KEY_A, [&] (int action) { camera->onKeyboard(movement::LEFT, action); });
  user.setKeyboardInputAction(GLFW_KEY_S, [&] (int action) { camera->onKeyboard(movement::BACKWARD, action); });
  user.setKeyboardInputAction(GLFW_KEY_D, [&] (int action) { camera->onKeyboard(movement::RIGHT, action); });
  user.setKeyboardInputAction(GLFW_KEY_SPACE, [&] (int action) {  });
  user.setKeyboardInputAction(GLFW_KEY_ESCAPE, [&] (int action) {engine.close();});
  user.setKeyboardInputAction(GLFW_KEY_L, [&] (int action) {
      scene.resetCamera();
    });
  
  Renderer renderer(&engine);
  Printer printer;
  //scene.print(printer);

  // main loop
  do {
    camera->update(float(glfwGetTime()-lastTime));
    scene.render(renderer);
    engine.update();
  } while(engine.isRunning());

}

