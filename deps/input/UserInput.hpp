#ifndef USERINPUT_HPP
#define USERINPUT_HPP

#include <map>
#include <functional>

class GLFWwindow;

class UserInput
{
public:
  UserInput() {}
  ~UserInput() {}

  bool init(GLFWwindow* window);
  bool setMouseInputAction(int input, std::function<void (void)> action);
  bool setKeyboardInputAction(int input, std::function<void (int)> action);
  bool setMousePosAction(std::function<void (double, double)> action) { this->mousePosChange = action; return true; }
  
private:
  static void mousePosCallbackImpl(GLFWwindow* window, double, double);
  void mousePosCallback(double, double);
  static void mouseButtonCallbackImpl(GLFWwindow* window, int, int, int);
  void mouseButtonCallback(int, int, int);
  static void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
  void keyCallback(int key, int scancode, int action, int mods);
  
  std::map<int, std::function<void (int)>> keyboardMap;
  std::map<int, std::function<void (void)>> mouseMap;

  std::function<void (double, double)> mousePosChange;
};

#endif
