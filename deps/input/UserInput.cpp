#include "UserInput.hpp"
#include <glfw3.h>

#include <iostream>
#include <sstream>

bool UserInput::init(GLFWwindow *window)
{
  
  glfwSetWindowUserPointer(window, this);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetMouseButtonCallback(window, mouseButtonCallbackImpl);
  glfwSetKeyCallback(window, keyCallbackImpl);
  glfwSetCursorPosCallback(window, mousePosCallbackImpl);
  glfwSetWindowSizeCallback(window, windowSizeCallbackImpl);
  return true;
}

void UserInput::mousePosCallbackImpl(GLFWwindow* window, double xpos, double ypos)
{
  UserInput* user = reinterpret_cast<UserInput*>(glfwGetWindowUserPointer(window));

  user->mousePosCallback(xpos, ypos);
  int width;
  int height;
  glfwGetWindowSize(window, &width, &height);
  //std::cout << "window size " << width << " " << height << std::endl;
  glfwSetCursorPos(window, width/2, height/2);
}

void UserInput::mousePosCallback(double xpos, double ypos)
{
  if(mousePosChange) {
    mousePosChange(xpos, ypos);
  }
}

void UserInput::windowSizeCallbackImpl(GLFWwindow *window, int width, int height)
{
  UserInput* user = reinterpret_cast<UserInput*>(glfwGetWindowUserPointer(window));
  user->windowSizeCallback(width, height);
}

void UserInput::windowSizeCallback(int width, int height)
{
  if(windowSizeChange) {
    windowSizeChange(width, height);
  }
}
  

void UserInput::mouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods)
{
  UserInput* user = reinterpret_cast<UserInput*>(glfwGetWindowUserPointer(window));

  user->mouseButtonCallback(button, action, mods);
}

void UserInput::mouseButtonCallback(int button, int action, int mods)
{
  std::stringstream ss;
  ss << button;
  
  //std::string input = "Mouse" + ss.str();
  std::map<int, std::function<void (void)>>::iterator it = mouseMap.find(button);
  
  // only activate action if mouse button is release
  if(action == 0) {
    if(it != mouseMap.end()) {
      (it->second)();
    } else {
      std::cerr << "Mouse " <<  button << " not found in config UserInput" << std::endl;
    }
  }
}

void UserInput::keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  //std::cout << key << std::endl;
  UserInput* user = reinterpret_cast<UserInput*>(glfwGetWindowUserPointer(window));

  user->keyCallback(key, scancode, action, mods);

}

void UserInput::keyCallback(int key, int scancode, int action, int mods)
{
  //std::cout << "key: " << key << " " << scancode << " " << action << " " << mods << std::endl;

  std::map<int, std::function<void (int)>>::iterator it = keyboardMap.find(key);
  if(it != keyboardMap.end()) {
    (it->second)(action);
  }else {
    std::cerr << "Key \"" << key << "\"" << " not found in config UserInput" << std::endl;
  }
}

bool UserInput::setMouseInputAction(int input, std::function<void (void)> action)
{
  mouseMap[input] = action;
  return true;
}

bool UserInput::setKeyboardInputAction(int input, std::function<void (int)> action)
{
  keyboardMap[input] = action;
  return true;
}

bool UserInput::setWindowSizeAction(std::function<void (int, int)> action)
{
  windowSizeChange = action;
  return true;
}
