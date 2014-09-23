#include "OGLRenderEngine.hpp"
#include <iostream>

#include "MathCore.hpp"
#include "CubeNode.hpp"
#include "ModelNode.hpp"
#include "loadShader.h"
#include "RenderContext.hpp"

#include "file_loader.h"
#include "UserInput.hpp"

#include "lodepng.h"

using namespace matc;

#define SHADER_PATH "../shader/"
#define TEXTURE_PATH "../assets/"

OGLRenderEngine::OGLRenderEngine(UserInput* user)
{
  init(user);
}

bool OGLRenderEngine::render(RenderContext& context, ModelNode& model) {
  std::map<std::string, BufferValues>::iterator it;
  
  it = bufferMap.find(model.getName());
  if (it == bufferMap.end()) {
    // std::cout << "need to init model" << std::endl;
    if(!initModel(model)) {
      std::cerr << "Error in init model" << std::endl;
      return false;
      }
    //    std::cout << "model initialized" << std::endl;
    it = bufferMap.find(model.getName());
  }

  /*
   * ======== SHADER ===========
   */
  GLuint programID;
  std::string shader = context.getShader();
  
  std::map<std::string, GLuint>::iterator shaderIt;
  shaderIt = shaderMap.find(shader);
  if (shaderIt == shaderMap.end()) {
    std::string vertexShader = SHADER_PATH + shader + ".v.glsl";
    std::string fragmentShader = SHADER_PATH + shader + ".f.glsl";
    programID = LoadShaders(vertexShader.c_str(), fragmentShader.c_str());
    shaderMap[shader] = programID;
    //std::cerr << "No shaderProgram found for model" << std::endl;
    //return false;
  } else {
    programID = shaderIt->second;
  }
  
  glUseProgram(programID);

  /*
   * ============ TEXTURE =========
   */
  std::string name = model.getName();
  GLuint textureBuffer;
  std::string texture = context.getTexture();
  //std::cout << "load texture: " << texture << std::endl;

  std::map<std::string, GLuint>::iterator textureIt;
  textureIt = textureMap.find(texture);
  
  if(textureIt == textureMap.end()) {
    std::string texturePath = TEXTURE_PATH + texture + ".png";
    std::cout << "OGLRENDERER: need to load texture: " << texturePath << std::endl;
    textureBuffer = loadPNG(texturePath.c_str());
    if(textureBuffer == -1) {
      std::cerr << "OGLRENDERER Couldn't load file: texturePath" << std::endl;
      return false;
    }
    textureMap[texture] = textureBuffer;
  } else {
    textureBuffer = textureIt->second;
  }
  
  /* 
   * ======= give the matrices to GLSL ==========
   */
  GLuint projMatrixID = glGetUniformLocation(programID, "ProjMatrix");
  GLuint viewMatrixID = glGetUniformLocation(programID, "ViewMatrix");
  GLuint modelMatrixID = glGetUniformLocation(programID, "ModelMatrix");

  glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, context.getProjection().asArray());
  glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, context.getView().asArray());
  glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, context.getModel().asArray());

  /*
   * ======== LIGHT ===============
   */
  GLuint lightColorID = glGetUniformLocation(programID, "gDirectionalLight.color");
  GLuint lightAmbientID = glGetUniformLocation(programID, "gDirectionalLight.ambientIntensity");
  GLuint diffuseID = glGetUniformLocation(programID, "gDirectionalLight.diffuseIntensity");
  GLuint directionID = glGetUniformLocation(programID, "gDirectionalLight.direction");
  Light light = context.getLight();
  Vector3 lightDirection = light.direction.normalize();

  //std::cout << lightDirection << std::endl;
  
  glUniform3f(lightColorID, light.color.x, light.color.y, light.color.z);
  glUniform1f(lightAmbientID, light.ambientIntensity);
  glUniform1f(diffuseID, light.diffuseIntensity);
  glUniform3f(directionID, lightDirection.x, lightDirection.y, lightDirection.z);

  GLuint vertexbuffer = (it->second).vertex;
  if (vertexbuffer == -1) {
    std::cerr << "No Vertex information found" << std::endl;
    return false;
  }
  
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			//0,
			//(void*)0
			sizeof(matc::Vector3),
			(void*)0
			);


  //GLuint textureBuffer = (it->second).texture;
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureBuffer);
  GLint uniform_texture = glGetUniformLocation(programID, "myTextureSampler");
  //std::cout << "textureID " << uniform_texture << std::endl;
  glUniform1i(uniform_texture, 0);
  
  GLuint uvBuffer = (it->second).uv;
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			//0,
			sizeof(matc::Vector2),
			(void*)0
			);

  GLuint normalBuffer = (it->second).normal;
  if(normalBuffer == -1) {
      std::cerr << "No normal information found" << std::endl;
      return false;
    }
  
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glVertexAttribPointer(2,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(matc::Vector3),
			(void*)0
			);

  GLuint elementBuffer = (it->second).element;
  if(elementBuffer == -1) {
    // std::cout << "rendering triangles: " << model.getTrianglesCount() << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, model.getTrianglesCount()*3);
  }
  
  return true;
}

bool OGLRenderEngine::render(RenderContext& context, CubeNode& cube)
{
  /*
    if "cube" already in "map": vlt id oder name vom cube?
        only render it, with saved buffers
    else:
        init gl_buffers
	save "cube" in "map"
	render saved buffers
  */
  std::map<std::string, BufferValues>::iterator it;
  
  it = bufferMap.find(cube.getName());
  if (it == bufferMap.end()) {
    std::cout << "need to init cube" << std::endl;
    if(!initModel(cube)) {
      std::cerr << "Error in init Cube" << std::endl;
      return false;
      }
    std::cout << "cube initialized" << std::endl;
    it = bufferMap.find(cube.getName());
  }
  
  std::map<std::string, GLuint>::iterator shaderIt;
  shaderIt = shaderMap.find(cube.getName());
  if (shaderIt == shaderMap.end()) {
    std::cerr << "No shaderProgram found for cube" << std::endl;
    return false;
  }
  
  GLuint programID = shaderIt->second;
  glUseProgram(programID);
  // give the matrices to GLSL
  GLuint projMatrixID = glGetUniformLocation(programID, "ProjMatrix");
  GLuint viewMatrixID = glGetUniformLocation(programID, "ViewMatrix");
  GLuint modelMatrixID = glGetUniformLocation(programID, "ModelMatrix");

  glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, context.getProjection().asArray());
  glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, context.getView().asArray());
  glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, context.getModel().asArray());

  GLuint vertexbuffer = (it->second).vertex;
  if (vertexbuffer == -1) {
    std::cerr << "No Vertex information found" << std::endl;
    return false;
  }
  
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);
  
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, (it->second).color);
  glVertexAttribPointer(
			1,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);

			

  GLuint elementbuffer = (it->second).element;
  if (elementbuffer != -1) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glDrawElements(
		   GL_TRIANGLES,
		   12*3,
		   GL_UNSIGNED_INT,
		   (void*)0
		   );
  } else {
    // Alternative?
    // noch magic number 12*3
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
  }
  glDisableVertexAttribArray(0);
  
  return true;
}

bool OGLRenderEngine::initModel(ModelNode& model)
{
  std::string name = model.getName();
  // shader hier laden?
  //GLuint programID = LoadShaders("../shader/texturedVertexShader.glsl", "../shader/texturedFragmentShader.glsl");
  //shaderMap[name] = programID;
  
  //std::vector<Vector4> verts = cube.getVertices();
  //std::vector<float> verts = model.getVertices();
  std::vector<matc::Vector3> verts = model.getVertices();
  GLuint vbo_cube_vertices;
  glGenBuffers(1, &vbo_cube_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
  glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(matc::Vector3), verts.data(), GL_STATIC_DRAW);

  //std::vector<float> uvs = model.getUV();
  std::vector<matc::Vector2> uvs = model.getUVs();
  GLuint uvBuffer;
  glGenBuffers(1, &uvBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(matc::Vector2), uvs.data(), GL_STATIC_DRAW);

  std::vector<matc::Vector3> normals = model.getNormals();
  //std::cout << "normals: " << normals.size() << std::endl;
  /*
  for(int i = 0; i < normals.size(); i++) {
    float test = normals[i].normalize().dot(Vector3(0, -1, -1));
    std::cout << test << std::endl;
  }
  */
  GLuint normalBuffer;
  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(matc::Vector3), normals.data(), GL_STATIC_DRAW);

  /*
  std::string texturePath = "../assets/" + name + ".png";
  GLuint textureBuffer;
  if(name == "ModelCube 1") {
    std::cout << "cube 1" << std::endl;
    textureBuffer = loadDDS("../assets/uvtemplate.DDS");
  } else {
    textureBuffer = loadDDS("../assets/uvmap.DDS");
  }
  */
  /*
  GLuint textureBuffer;
  if(name == "ModelCube 1") {
    textureBuffer = loadPNG("../assets/cube.png");
  } else if(name == "Skybox_ground") {
    textureBuffer = loadPNG("../assets/Skybox_ground.png");
  } else if(name == "Skybox_sky") {
    textureBuffer = loadPNG("../assets/Skybox_sky.png");
  }else {
    textureBuffer = loadPNG("../assets/sample.png");
  }
  */
  //GLuint textureBuffer = loadPNG(texturePath.c_str());
  // if(name == "Skybox_sky") textureBuffer = 2;
  
  //std::cout << "texid for " << name << ": " << textureBuffer << std::endl;
  
  
  
  BufferValues buffers;
  buffers.vertex = vbo_cube_vertices;
  buffers.uv = uvBuffer;
  buffers.normal = normalBuffer;
  //buffers.texture = textureBuffer;

  bufferMap[name] = buffers;
  return true;
}

bool OGLRenderEngine::initModel(CubeNode& cube)
{
  std::string name = cube.getName();
  // shader hier laden?
  GLuint programID = LoadShaders("../shader/SimpleVertexShader.glsl", "../shader/SimpleFragmentShader.glsl");
  shaderMap[name] = programID;
  
  //std::vector<Vector4> verts = cube.getVertices();
  std::vector<float> verts = cube.getVertices();
  GLuint vbo_cube_vertices;
  glGenBuffers(1, &vbo_cube_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
  glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(float), verts.data(), GL_STATIC_DRAW);

  BufferValues buffers;
  buffers.vertex = vbo_cube_vertices;

  
  std::vector<Vector4> col = cube.getColors();
  Vector4 *colors = col.data();
  GLuint colorBuffer;
  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * col.size(), col.data(), GL_STATIC_DRAW);
  buffers.color = colorBuffer;

  std::vector<unsigned int> elements = cube.getElements();
  GLuint elementBuffer;
  glGenBuffers(1, &elementBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
  buffers.element = elementBuffer;
  
  bufferMap[name] = buffers;
  return true;
}

bool OGLRenderEngine::init(UserInput* user)
{
  
  if(!glfwInit()){
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return false;
  }
    
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
 
  window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
  if(window == NULL) {
    std::cerr << "Failed to open GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glewExperimental=true; // wirklich benoetigt?
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to init GLEW" << std::endl;
    return false;
  }
  
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  glEnable(GL_TEXTURE_2D);

  // Init VertexArray sicher hier?
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  user->init(window);
}

void OGLRenderEngine::update()
{
  glfwSwapBuffers(window);
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderEngine::close()
{
  glfwSetWindowShouldClose(window, GL_TRUE);
}

bool OGLRenderEngine::isRunning()
{
  //return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
  return glfwWindowShouldClose(window) == 0;
}
