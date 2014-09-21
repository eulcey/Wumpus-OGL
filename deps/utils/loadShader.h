#ifndef LOADSHADER_H
#define LOADSHADER_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include <fstream>
#include <ios>
#include <algorithm>


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

//GLuint loadTGA_glfw(const char *imagepath);

#endif
