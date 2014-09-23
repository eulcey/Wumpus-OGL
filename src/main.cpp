#include <iostream>
#include "MathCore.hpp"
#include "Printer.hpp"
#include "Renderer.hpp"
#include "CubeNode.hpp"
#include "ModelNode.hpp"
#include "CameraNode.hpp"
#include "OGLRenderEngine.hpp"
#include "UserInput.hpp"
#include "Scene.hpp"
//#include "Camera.hpp"
//#include "World.hpp"

using namespace matc;
using namespace std;

const int screen_width = 1024;
const int screen_height = 768;
const float mouseSpeed = 0.0005;
const float speed = 0.01;

//Matrix4x4 computeTranslation(int, double, Vector3&);
//Matrix4x4 computeView(double, double, float&, float&, double, Vector3);

Vector3 globalDirection;
Vector3 globalRight;

int main(void)
{

  //TransformNode root("Root", Matrix4x4());
  MaterialNode root("Rootmaterial", "cube","texturedShader");

  
  // Camera camera("Camera 1", screen_width, screen_height);
  //camera.link(root);
  //Camera camera2("Camera 2", screen_width, screen_height, glfwGetTime());
  //camera2.link(root);
  
  CubeNode cube("Cube");
  TransformNode cube1Translation("Translation 1", translate(Matrix4x4(), Vector3(0.0, 0.0, -2.0)));
  TransformNode cube1Rotation("Rotation 1", rotate(Matrix4x4(), 3.14/4, Vector3(0.0, 1.0, 0.0)));

  TransformNode cube1Scale("Scale 1", scale(Matrix4x4(), 0.5f, 0.5f, 0.5f));

  TransformNode cube2Translation("Translation 2", translate(Matrix4x4(), Vector3(-2.0, 0.0, 1.0)));
  TransformNode cube2Scale("Scale 2", scale(Matrix4x4(),1.0f, 1.0f, 1.0f));

  ModelNode modelCube1("ModelCube 1", "../assets/cube.obj");
  MaterialNode modelCube1_material("ModelCube 1 Material", "sample", "texturedShader");
  ModelNode modelCube2("ModelCube 2", "../assets/cube.obj");
  MaterialNode modelCube2_material("ModelCube 2 Material", "cube", "texturedShader");
  //ModelNode modelCube("Monkey", "../assets/monkey.obj");
  ModelNode skybox_ground("Skybox_ground", "../assets/skybox_ground.obj");
  MaterialNode skybox_ground_material("Skybox Ground Material", "Skybox_ground", "texturedShader");
  skybox_ground_material.addChild(&skybox_ground);
  ModelNode skybox_sky("Skybox_sky", "../assets/skybox_sky.obj");
  MaterialNode skybox_sky_material("Skybox Sky Material", "Skybox_sky", "texturedShader");
  skybox_sky_material.addChild(&skybox_sky);

  TransformNode skyboxScale("Skybox Scale", scale(Matrix4x4(), 10.0f, 10.0f, 10.0f));
  skyboxScale.addChild(&skybox_sky_material);
  //skyboxScale.addChild(&skybox_ground);
  /*
  TransformNode groundTrans("Ground Translation", translate(Matrix4x4(), Vector3(0.0, -0.2f, 0.0)));
  TransformNode groundScale("Ground Scale", scale(Matrix4x4(), 10.0f, 0.1f, 10.0f));
  groundScale.addChild(&cube);
  groundTrans.addChild(&groundScale);
  //skyboxScale.addChild(&groundTrans);
  root.addChild(&groundTrans);
  //cameraPos.addChild(&skyboxScale);
  */
  //camera2.addSkybox(skyboxScale);

  
  //root.addChild(&cube1Translation);
  //cube1Translation.addChild(&cube1Rotation);
  //cube1Rotation.addChild(&modelCube2_material);
  
    root.addChild(&cube2Translation);
  cube2Translation.addChild(&modelCube1_material);

  modelCube1_material.addChild(&modelCube1);
  modelCube2_material.addChild(&modelCube1);
  

  // World world = loadWorld("..");
  //world.linkWorld(root);
  
  
  UserInput user;
  Scene scene(screen_width, screen_height);
  scene.load("");

  Camera *camera = scene.getCamera();
  
  float horizontalAngle = 3.14f;
  float verticalAngle = 0.0f;
  float lastTime = glfwGetTime();
  
  OGLRenderEngine engine(&user);
  
  user.setMouseInputAction(GLFW_MOUSE_BUTTON_LEFT, [] { std::cout << "left mouse button clicked" << std::endl; });
  user.setMouseInputAction(GLFW_MOUSE_BUTTON_RIGHT, [] { std::cout << "right mouse button clicked" << std::endl; });
  
  user.setMousePosAction([&] (double xpos, double ypos) {
      //viewMatrix = computeView(xpos, ypos, horizontalAngle, verticalAngle, lastTime, position).invert();
      camera->changeView(xpos, ypos, glfwGetTime());
    });
  /*
  user.setKeyboardInputAction(GLFW_KEY_W, [&] { posMatrix = computeTranslation(GLFW_KEY_W, lastTime, position); });
  user.setKeyboardInputAction(GLFW_KEY_A, [&] { posMatrix = computeTranslation(GLFW_KEY_A, lastTime, position); });
  user.setKeyboardInputAction(GLFW_KEY_S, [&] { posMatrix = computeTranslation(GLFW_KEY_S, lastTime, position); });
  user.setKeyboardInputAction(GLFW_KEY_D, [&] { posMatrix = computeTranslation(GLFW_KEY_D, lastTime, position); });
  */
  
  user.setKeyboardInputAction(GLFW_KEY_W, [&] (int action) { camera->onKeyboard(movement::FORWARD, action); });
  user.setKeyboardInputAction(GLFW_KEY_A, [&] (int action) { camera->onKeyboard(movement::LEFT, action); });
  user.setKeyboardInputAction(GLFW_KEY_S, [&] (int action) { camera->onKeyboard(movement::BACKWARD, action); });
  user.setKeyboardInputAction(GLFW_KEY_D, [&] (int action) { camera->onKeyboard(movement::RIGHT, action); });
  user.setKeyboardInputAction(GLFW_KEY_SPACE, [&] (int action) {  });
  user.setKeyboardInputAction(GLFW_KEY_ESCAPE, [&] (int action) {engine.close();});
  
  Renderer renderer(&engine);
  Printer printer;
  //root.accept(printer);
  scene.print(printer);


  //std::cout << "Viewmatrix: " << viewMatrix << std::endl;
  // main loop
  do {
    //view.setTransform(viewMatrix);
    //cameraPos.setTransform(posMatrix);
    camera->update(glfwGetTime());
    //root.accept(renderer);
    scene.render(renderer);
    engine.update();
  } while(engine.isRunning());
}

/*
Matrix4x4 computeTranslation(int button, double lastTime, Vector3 &position)
{
  double currentTime = glfwGetTime();
  float deltaTime = float(currentTime - lastTime);
  Vector3 direction;
  if(button == GLFW_KEY_W) {
    direction = globalDirection * -1;
  } else if(button == GLFW_KEY_S) {
    direction = globalDirection;
  } else if(button == GLFW_KEY_D) {
    direction = globalRight * -1;
  } else {
    direction = globalRight;
  }
  position = position + direction * deltaTime * speed;
  return translate(Matrix4x4(), Vector4(position.x, position.y, position.z, 0)).invert();
}

Matrix4x4 computeView(double xpos, double ypos, float &horizontalAngle, float &verticalAngle, double lastTime, Vector3 position)
{
  double currentTime = glfwGetTime();
  float deltaTime = float(currentTime - lastTime);
  
  horizontalAngle += mouseSpeed * deltaTime * float(screen_width/2 - xpos);
  verticalAngle += mouseSpeed * deltaTime * float(screen_height/2 - ypos);
  Vector3 direction(cos(verticalAngle) * sin(horizontalAngle),
		    sin(verticalAngle),
		    cos(verticalAngle) * cos(horizontalAngle)
		    );
  Vector3 right(sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
		);
  Vector3 up = right.cross(direction);

  globalDirection = direction;
  globalRight = right;

  return lookAt(Vector3(0.0, 0.0, 0.0), direction, up);
  //return rotate(rotate(Matrix4x4(), horizontalAngle, Vector3(0.0, 1.0, 0.0)), verticalAngle, Vector3(1.0, 0.0, 0.0)).transpose();
}
*/
