#ifndef CURSORRENDERER_HPP
#define CURSORRENDERER_HPP

#include <string>

//namespace util {
  
  bool initCursor2D(const std::string &texturePath, const std::string &shader);
  void drawCursor2D(float x, float y, int size);
  void cleanupCursor2D();
  
//} // namespace util

#endif
