#ifndef TEXTRENDERER_HPP
#define TEXTRENDERER_HPP

#include <string>

//namespace util {
  
  bool initText2D(const std::string &texturePath, const std::string &shader);
  void printText2D(const std::string &text, int x, int y, int size);
  void cleanupText2D();
  
//} // namespace util

#endif
