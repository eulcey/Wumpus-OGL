#include "TextRenderer.hpp"
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

GLuint text2DTextureID;
GLuint text2DVertexBufferID;
GLuint text2DUVBufferID;
GLuint text2DShaderID;
GLuint text2DUniformID;

bool initText2D(const std::string &texturePath, const std::string &textShader)
{
  text2DTextureID = loadPNG(texturePath.c_str());
 
  glGenBuffers(1, &text2DVertexBufferID);
  glGenBuffers(1, &text2DUVBufferID);

  std::string vertexShader = SHADER_PATH + textShader + ".v.glsl";
  std::string fragShader = SHADER_PATH + textShader + ".f.glsl";
  text2DShaderID = LoadShaders(vertexShader.c_str(), fragShader.c_str());

  text2DUniformID = glGetUniformLocation(text2DShaderID, "myTextureSampler");

  return true;
}


void printText2D(const std::string &text, int x, int y, int size)
{
  std::vector<Vector2> vertices;
  std::vector<Vector2> uvs;

  for(size_t i = 0; i < text.size(); i++) {
    Vector2 v_up_left = Vector2(x + i*size, y + size);
    Vector2 v_up_right = Vector2(x + i*size + size, y + size);
    Vector2 v_down_right = Vector2(x + i*size + size, y);
    Vector2 v_down_left = Vector2(x + i*size, y);

    vertices.push_back(v_up_left);
    vertices.push_back(v_down_left);
    vertices.push_back(v_up_right);

    vertices.push_back(v_down_right);
    vertices.push_back(v_up_right);
    vertices.push_back(v_down_left);

    char character = text[i];
    int pos = character - 32;
    float uv_x = (pos%8)/8.0f;
    float uv_y = (pos/8)/8.0f;

    Vector2 uv_up_left = Vector2(uv_x, uv_y);
    Vector2 uv_up_right = Vector2(uv_x + 1.0f/8.0f, uv_y);
    Vector2 uv_down_right = Vector2(uv_x + 1.0f/8.0f, (uv_y + 1.0f/8.0f));
    Vector2 uv_down_left = Vector2(uv_x,  (uv_y + 1.0f/8.0f));

    uvs.push_back(uv_up_left);
    uvs.push_back(uv_down_left);
    uvs.push_back(uv_up_right);

    uvs.push_back(uv_down_right);
    uvs.push_back(uv_up_right);
    uvs.push_back(uv_down_left);
  }

  glBindBuffer(GL_ARRAY_BUFFER, text2DVertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(matc::Vector2), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, text2DUVBufferID);
  glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(matc::Vector2), uvs.data(), GL_STATIC_DRAW);

  glUseProgram(text2DShaderID);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, text2DTextureID);
  glUniform1i(text2DUniformID, 0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, text2DVertexBufferID);
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
  glBindBuffer(GL_ARRAY_BUFFER, text2DUVBufferID);
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
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  glDisable(GL_BLEND);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void cleanupText2D()
{
  glDeleteBuffers(1, &text2DVertexBufferID);
  glDeleteBuffers(1, &text2DUVBufferID);

  glDeleteTextures(1, &text2DTextureID);

  glDeleteProgram(text2DShaderID);
}
