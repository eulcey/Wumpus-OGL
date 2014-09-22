#include "Scene.hpp"

#include <iostream>
#include "MathCore.hpp"
#include "Printer.hpp"
#include "Renderer.hpp"
#include "OGLRenderEngine.hpp"
#include "Camera.hpp"

using namespace matc;

Scene::Scene(int width, int height): camera(Camera("SceneCamera", width, height, 0))
{
  root = new TransformNode("SceneRoot", Matrix4x4());
}
