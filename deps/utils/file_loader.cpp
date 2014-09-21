#include "file_loader.h"

#include <iostream>
#include <cstring>

#include <vector>
#include <string>
#include <fstream>
#include <ios>
#include <algorithm>
#include "lodepng.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

char* file_read(const char* filename)
{
  FILE* input = fopen(filename, "rb");
  if(input == NULL) return NULL;

  if(fseek(input, 0, SEEK_END) == -1) return NULL;
  long size = ftell(input);
  if(size == -1) return NULL;
  if(fseek(input, 0, SEEK_SET) == -1) return NULL;

  char *content = (char*) malloc( (size_t) size+1);
  if(content == NULL) return NULL;

  fread(content, 1, (size_t) size, input);
  if(ferror(input)) {
    free(content);
    return NULL;
  }
  fclose(input);
  content[size] = '\0';
  return content;
}

/**
 * display errors from shader compiler
 */
void print_log(GLuint object)
{
  GLint log_length = 0;
  if (glIsShader(object))
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else if(glIsProgram(object))
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else {
    std::cerr << "printlog: Not a shader or a program\n" << std::endl;
    return;
  }

  char *log = (char*)malloc(log_length);
  if(glIsShader(object))
    glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
    glGetProgramInfoLog(object, log_length, NULL, log);

  std::cerr << log << std::endl;
  free(log);
}

GLuint loadBMP_custom(const char *imagepath)
{
  unsigned char header[54];
  unsigned int dataPos;
  unsigned int width, height;
  unsigned int imageSize; // = width*height*3
  //actual rgb data
  unsigned char *data;

  FILE *file = fopen(imagepath, "rb");
  if (!file) { std::cerr << "Image could not be opened" << std::endl; return -1; }

  if (fread(header, 1, 54, file) != 54)
    {
      std::cerr << "Not a correct BMP file" << std::endl;
      return -1;
    }

  if(header[0] != 'B' || header[1] != 'M') { std::cerr << "Not a correct BMP file" << std::endl; return -1; }

  dataPos = *(int*)&(header[0x0A]);
  imageSize = *(int*)&(header[0x22]);
  width = *(int*)&(header[0x12]);
  height = *(int*)&(header[0x16]);

  imageSize = imageSize?imageSize:width*height*3;
  dataPos = dataPos?dataPos:54;

  data = new unsigned char[imageSize];

  fread(data, 1, imageSize, file);
  fclose(file);

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  return textureID;
}

GLuint loadDDS(const char *imagepath)
{
  unsigned char header[124];

  FILE *fp;

  fp = fopen(imagepath, "rb");
  if (fp == NULL)
    return -1;

  char filecode[4];
  fread(filecode, 1, 4, fp);
  if (strncmp(filecode, "DDS ", 4) != 0)
    {
      fclose(fp);
      return -1;
    }

  fread(&header, 124, 1, fp);

  unsigned int height      = *(unsigned int*)&(header[8 ]);
  unsigned int width         = *(unsigned int*)&(header[12]);
  unsigned int linearSize     = *(unsigned int*)&(header[16]);
  unsigned int mipMapCount = *(unsigned int*)&(header[24]);
  unsigned int fourCC      = *(unsigned int*)&(header[80]);

  unsigned char *buffer;
  unsigned int bufsize;

  bufsize = mipMapCount > 1? linearSize*2: linearSize;
  buffer = (unsigned char*) malloc(bufsize * sizeof(unsigned char));
  fread(buffer, 1, bufsize, fp);
  fclose(fp);

  unsigned int components = (fourCC == FOURCC_DXT1)? 3 : 4;
  unsigned int format;
  switch(fourCC)
    {
    case FOURCC_DXT1:
      format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      break;
     case FOURCC_DXT3:
      format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      break;
     case FOURCC_DXT5:
      format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      break;
    default:
      free(buffer);
      return -1;
    }

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  
  unsigned int blocksize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)?8:16;
  unsigned int offset = 0;

  /*load mipmaps*/
  for(unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
      unsigned int size = ((width+3)/4)*((height+3)/4)*blocksize;
      glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

      offset += size;
      width /= 2;
      height /= 2;
    }
  free(buffer);

  return textureID;
  
}

/*
GLuint loadTGA_glfw(const char *imagepath)
{
  GLuint textureID;
  glGenTextures(1, &textureID);

  glBindTexture(GL_TEXTURE_2D, textureID);
  glfwLoadTexture2D(imagepath, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);

  return textureID;
}
*/

void flip_image_vertical(
    unsigned char *data, unsigned int width, unsigned int height)
{
    unsigned int size = width * height * 4;
    unsigned int stride = sizeof(char) * width * 4;
    unsigned char *new_data = (unsigned char*)malloc(sizeof(unsigned char) * size);
    for (unsigned int i = 0; i < height; i++) {
        unsigned int j = height - i - 1;
        memcpy(new_data + j * stride, data + i * stride, stride);
    }
    memcpy(data, new_data, size);
    free(new_data);
}

GLuint loadPNG(const char *imagepath)
{
  unsigned int error;
  unsigned char *data;
  unsigned width, height;
  error = lodepng_decode32_file(&data, &width, &height, imagepath);
  if (error) {
    //fprintf(stderr, "error %u: %s\n", error, lodepng_error_text(error));
    std::cerr << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    return -1;
  }
  //flip_image_vertical(data, width, height);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  free(data);
  
  return textureID;
}
