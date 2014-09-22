#ifndef OGLRENDERENGINE_HPP
#define OGLRENDERENGINE_HPP

#include <map>
#include <string>

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class UserInput;

class RenderContext;
class CubeNode;
class ModelNode;
//class GLFWwindow;

struct BufferValues {
  GLuint vertex = -1;
  GLuint color = -1;
  GLuint element = -1;
  //GLuint texture = -1;
  GLuint uv = -1;
  //GLuint program = -1;
};

class OGLRenderEngine
{
public:
  OGLRenderEngine(UserInput* user);
  ~OGLRenderEngine() { }

  bool render(RenderContext& context, CubeNode& cube);
  bool render(RenderContext& context, ModelNode& model);

  bool initModel(ModelNode& model);
  bool initModel(CubeNode& cube);

  void update();
  void close();
  bool isRunning();
private:
  bool init(UserInput* user);
  GLFWwindow* window;
  std::map<std::string, BufferValues> bufferMap;
  std::map<std::string, GLuint> shaderMap;
  std::map<std::string, GLuint> textureMap;
};
#endif
