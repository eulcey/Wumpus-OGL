#include <iostream>
#include "MathCore.hpp"

#include "Printer.hpp"
#include "Renderer.hpp"
#include "ShadowRenderer.hpp"
#include "OGLRenderEngine.hpp"
#include "UserInput.hpp"
#include "Scene.hpp"
using namespace matc;
using namespace std;

const int target_width = 820; // 1200
const int target_height = 700; // 1024
const float mouseSpeed = 0.0005;
const float speed = 0.01;


Vector3 globalDirection;
Vector3 globalRight;

int main(int argc, char** argv)
{
  UserInput user;
  OGLRenderEngine engine(target_width, target_height, "Wumpus-OGL", &user);
  Vector2i win_size = engine.getRealWindowSize();
  int screen_width = target_width;//win_size.x;
  int screen_height = target_height;//win_size.y;
  Scene scene(screen_width, screen_height, &user);
  
  std::string filepath;
  
  if(argc >= 2) {
    filepath = argv[1];
    std::cout << "loading: " << argv[1] << std::endl;
  } else {
    filepath = "../saved_worlds/original_world";
  }
  
  if(!scene.load(filepath)) {
    std::cerr << "Error while loading: " << filepath << std::endl;
    return -1;
  }

  Camera *camera = scene.getCamera();
  
  float lastTime = glfwGetTime();
  
  
  if(!engine.shadowInit()) {
    std::cout << "Error in Shadow init" << std::endl;
  } else {
    std::cout << "Shadow Init successful" << std::endl;
  }
  
  user.setMouseInputAction(GLFW_MOUSE_BUTTON_LEFT, [&] { scene.clickCursor(); });
  user.setMouseInputAction(GLFW_MOUSE_BUTTON_RIGHT, [] { std::cout << "right mouse button clicked" << std::endl; });


  user.setKeyboardInputAction(GLFW_KEY_F, [&] (int action) { if(action) scene.switchMouseLook(); });


  user.setKeyboardInputAction(GLFW_KEY_W, [&] (int action) { camera->onKeyboard(movement::FORWARD, action); });
  user.setKeyboardInputAction(GLFW_KEY_A, [&] (int action) { camera->onKeyboard(movement::LEFT, action); });
  user.setKeyboardInputAction(GLFW_KEY_S, [&] (int action) { camera->onKeyboard(movement::BACKWARD, action); });
  user.setKeyboardInputAction(GLFW_KEY_D, [&] (int action) { camera->onKeyboard(movement::RIGHT, action); });
  user.setKeyboardInputAction(GLFW_KEY_SPACE, [&] (int action) { });
  user.setKeyboardInputAction(GLFW_KEY_ESCAPE, [&] (int action) {engine.close();});
  user.setKeyboardInputAction(GLFW_KEY_L, [&] (int action) {
      scene.resetCamera();
    });
  
  Renderer renderer(&engine);
  ShadowRenderer shadowRenderer(&engine);
  Printer printer;
  //scene.print(printer);

  scene.initTextRender();
  // main loop
  do {
    
    // engine.startShadowRender();
    //scene.render(shadowRenderer);
    
    engine.startRender();
    scene.render(renderer);
    scene.update(float(glfwGetTime()-lastTime));
    
    engine.update(); // here swapping buffers
  } while(engine.isRunning());

  //scene.deleteScene();
  //std::cout << "Scene deleted" << std::endl;
}

