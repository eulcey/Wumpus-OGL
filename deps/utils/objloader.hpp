#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "MathCore.hpp"
#include <vector>

bool loadOBJ(const char* path,
	     std::vector<matc::Vector3> &out_vertices,
	     std::vector<matc::Vector2> &out_uvs,
	     std::vector<matc::Vector3> &out_normals
	     );

#endif
