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

OGLRenderEngine::OGLRenderEngine(int width, int height, std::string title, UserInput* user)
{
  init(width, height, title, user);
}

OGLRenderEngine::~OGLRenderEngine()
{
  /**
   * TODO: delete all buffers
   */
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

    std::cout << "for shader: " << shader << std::endl;
    GLuint shadowMapID = glGetUniformLocation(programID, "shadowMap");
    std::cout << shadowMapID << std::endl;
    
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
  GLint textureBuffer;
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
  GLuint specularPowerID = glGetUniformLocation(programID, "gSpecularPower");
  GLuint specularIntensityID = glGetUniformLocation(programID, "gSpecularIntensity");
  GLuint eyeWorldID = glGetUniformLocation(programID, "gEyeWorldPos");
  
  Light light = context.getLight();
  Vector3 lightDirection = light.direction.normalize();
  
  glUniform3f(lightColorID, light.color.x, light.color.y, light.color.z);
  glUniform1f(lightAmbientID, light.ambientIntensity);
  glUniform1f(diffuseID, light.diffuseIntensity);
  glUniform3f(directionID, lightDirection.x, lightDirection.y, lightDirection.z);
  Vector3 cameraPosition = context.getCameraPosition();
  glUniform3f(eyeWorldID, cameraPosition.x, cameraPosition.y, cameraPosition.z);
  glUniform1f(specularPowerID, context.specularPower);
  glUniform1f(specularIntensityID, context.specularIntensity);

  GLint vertexbuffer = (it->second).vertex;
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

  /*
   * =========== Shadow =============
   */
  // Only if shadowInit() successful
  if(shader == "SimpleShadowShader"){//(renderWithShadows) {
    
    Matrix4x4 biasMatrix = translate(scale(Matrix4x4(), 0.5, 0.5, 0.5), Vector3(0.5, 0.5, 0.5));
    Matrix4x4 depthBiasMVP = biasMatrix * depthMVP;

    GLuint depthBiasID = glGetUniformLocation(programID, "depthBiasMVP");
    GLint shadowMapID = glGetUniformLocation(programID, "shadowMap");
    //std::cout << shadowMapID << std::endl;

    glUniformMatrix4fv(depthBiasID, 1, GL_FALSE, depthBiasMVP.asArray());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(shadowMapID, 1);
  }
  
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

  GLint normalBuffer = (it->second).normal;
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

  GLint elementBuffer = (it->second).element;
  if(elementBuffer == -1) {
    // std::cout << "rendering triangles: " << model.getTrianglesCount() << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, model.getTrianglesCount()*3);
  }
  
  return true;
}

bool OGLRenderEngine::render(RenderContext& context, CubeNode& cube)
{
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

  GLint vertexbuffer = (it->second).vertex;
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

			

  GLint elementbuffer = (it->second).element;
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

  GLuint normalBuffer;
  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(matc::Vector3), normals.data(), GL_STATIC_DRAW);

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
  //Vector4 *colors = col.data();
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

bool OGLRenderEngine::init(int width, int height, std::string title, UserInput* user)
{
  this->width = width;
  this->height = height;
  if(!glfwInit()){
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return false;
  }
    
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
 
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
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


  return user->init(window);
}

bool OGLRenderEngine::shadowInit()
{
  depthProgramID = LoadShaders("../shader/DepthRTT.v.glsl", "../shader/DepthRTT.f.glsl");
  depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");
  
  framebufferName = 0;
  glGenFramebuffers(1, &framebufferName);
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
  
  glGenTextures(1, &depthTexture);
  glBindTexture(GL_TEXTURE_2D, depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
  glDrawBuffer(GL_NONE);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  renderWithShadows = true;
  
  return true;
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


bool OGLRenderEngine::renderShadow(RenderContext& context, CubeNode& cube)
{
  return true;
}

void OGLRenderEngine::startRender()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // Render on the whole framebuffer, complete from the lower left corner to the upper right
  glViewport(0,0,width,height);
  
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderEngine::startShadowRender()
{
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
  glViewport(0, 0, 1024, 1024);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(depthProgramID);

}

bool OGLRenderEngine::renderShadow(RenderContext& context, ModelNode& model)
{
    
  Vector3 lightInvDir = context.getLight().direction * -1;
  Matrix4x4 projection = context.getProjection();
  Matrix4x4 view = context.getView();
  Matrix4x4 modelIdent = context.getModel();
  depthMVP = projection * view * modelIdent;

  glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, depthMVP.asArray());

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
  
  GLint vertexbuffer = (it->second).vertex;
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
  
  glDrawArrays(GL_TRIANGLES, 0, model.getTrianglesCount()*3);

  glDisableVertexAttribArray(0);
  
  return true;
}
