#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <GL/glew.h>

char* file_read(const char* filename);

void print_log(GLuint object);

GLuint loadBMP_custom(const char *imagepath);

GLuint loadDDS(const char *imagepath);

GLuint loadPNG(const char *imagepath);

//GLuint loadTGA_glfw(const char *imagepath);

#endif
