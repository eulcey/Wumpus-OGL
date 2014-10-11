#include "CursorRenderer.hpp"
#include "file_loader.h"
#include "loadShader.h"
#include "MathCore.hpp"

#include <vector>

#if(USE_ES_SHADER)
  #define SHADER_PATH "../es_shader/"
#else
  #define SHADER_PATH "../shader/"
#endif

//using namespace util;
using namespace matc;

GLuint cursor2DTextureID;
GLuint cursor2DVertexBufferID;
GLuint cursor2DUVBufferID;
GLuint cursor2DShaderID;
GLuint cursor2DUniformID;

bool initCursor2D(const std::string &texturePath, const std::string &cursorShader)
{
  cursor2DTextureID = loadPNG(texturePath.c_str());
 
  glGenBuffers(1, &cursor2DVertexBufferID);
  glGenBuffers(1, &cursor2DUVBufferID);

  std::string vertexShader = SHADER_PATH + cursorShader + ".v.glsl";
  std::string fragShader = SHADER_PATH + cursorShader + ".f.glsl";
  cursor2DShaderID = LoadShaders(vertexShader.c_str(), fragShader.c_str());

  cursor2DUniformID = glGetUniformLocation(cursor2DShaderID, "myTextureSampler");

  return true;
}


void drawCursor2D(float x, float y, int size)
{
  std::vector<Vector2> vertices;
  std::vector<Vector2> uvs;
  
  Vector2 v_up_left = Vector2(x + size, y + size);
  Vector2 v_up_right = Vector2(x + size + size, y + size);
  Vector2 v_down_right = Vector2(x + size + size, y);
  Vector2 v_down_left = Vector2(x + size, y);
  
  vertices.push_back(v_up_left);
  vertices.push_back(v_down_left);
  vertices.push_back(v_up_right);
  
  vertices.push_back(v_down_right);
  vertices.push_back(v_up_right);
  vertices.push_back(v_down_left);

  float uv_x = 0.0f;
  float uv_y = 0.0f;
  
  Vector2 uv_up_left = Vector2(uv_x, uv_y);
  Vector2 uv_up_right = Vector2(uv_x + 1.0f, uv_y);
  Vector2 uv_down_right = Vector2(uv_x + 1.0f, (uv_y + 1.0f));
  Vector2 uv_down_left = Vector2(uv_x,  (uv_y + 1.0f));
  
  uvs.push_back(uv_up_left);
  uvs.push_back(uv_down_left);
  uvs.push_back(uv_up_right);
  
  uvs.push_back(uv_down_right);
  uvs.push_back(uv_up_right);
  uvs.push_back(uv_down_left);

  glBindBuffer(GL_ARRAY_BUFFER, cursor2DVertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(matc::Vector2), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, cursor2DUVBufferID);
  glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(matc::Vector2), uvs.data(), GL_STATIC_DRAW);

  glUseProgram(cursor2DShaderID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cursor2DTextureID);
  glUniform1i(cursor2DUniformID, 0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, cursor2DVertexBufferID);
  glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			//0,
			//(void*)0
			sizeof(matc::Vector2),
			(void*)0
			);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, cursor2DUVBufferID);
  glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			//0,
			//(void*)0
			sizeof(matc::Vector2),
			(void*)0
			);

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void cleanupCursor2D()
{
  glDeleteBuffers(1, &cursor2DVertexBufferID);
  glDeleteBuffers(1, &cursor2DUVBufferID);

  glDeleteTextures(1, &cursor2DTextureID);

  glDeleteProgram(cursor2DShaderID);
}
