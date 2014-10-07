#include "objloader.hpp"

#include <cstring>
using namespace matc;

//#define GLM_FORCE_RADIANS
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <iostream>

bool loadOBJ(const char* path,
	     std::vector<matc::Vector3> &out_vertices,
	     std::vector<matc::Vector2> &out_uvs,
	     std::vector<matc::Vector3> &out_normals
	     )
{

  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

  std::vector<Vector3> temp_vertices, temp_normals;
  std::vector<Vector2> temp_uvs;
//  std::vector<glm::vec3> glm_vertices, glm_normals;
//  std::vector<glm::vec2> glm_uvs;
  
  FILE *file = fopen(path, "r");
  if(file == NULL)
    {
      printf("Error opening file %s", path);
      return false;
    }

  char lineHeader[128];
  int res = fscanf(file, "%s", lineHeader);
  while(res != EOF)
    {
      if(strcmp(lineHeader, "v") == 0)
	{
	  Vector3 vertex;
	  float x, y, z;
	  fscanf(file, "%f %f %f\n", &x, &y, &z);
	  vertex.x = x; vertex.y = y; vertex.z = z;
	  temp_vertices.push_back(vertex);
	  /* glm::vec3 glm_v;
	  glm_v.x = x; glm_v.y = y; glm_v.z = z;
	  glm_vertices.push_back(glm_v);*/
	  
	}
      else if(strcmp(lineHeader, "vt") == 0)
	{
	  Vector2 uv;
	  float x,y;
	  fscanf(file, "%f %f\n", &x, &y);
	  uv.x = x; uv.y = -y;
	  temp_uvs.push_back(uv);
	  /*glm::vec2 glm_uv;
	  glm_uv.x = x; glm_uv.y = y;
	  glm_uvs.push_back(glm_uv);*/
	}
      else if(strcmp(lineHeader, "vn") == 0)
	{
	  Vector3 normal;
	  fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
	  temp_normals.push_back(normal);
	}
      else if(strcmp(lineHeader, "f") == 0)
	{
	  std::string vertex1, vertex2, vertex3;
	  unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	  int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
	  if(matches != 9)
	    {
	      printf("File can't be read by parser (Try other export options), matches was: %d\n", matches);
	      return false;
	    }
	  vertexIndices.push_back(vertexIndex[0]);
	  vertexIndices.push_back(vertexIndex[1]);
	  vertexIndices.push_back(vertexIndex[2]);
	  uvIndices.push_back(uvIndex[0]);
	  uvIndices.push_back(uvIndex[1]);
	  uvIndices.push_back(uvIndex[2]);
	  normalIndices.push_back(normalIndex[0]);
	  normalIndices.push_back(normalIndex[1]);
	  normalIndices.push_back(normalIndex[2]);
	}
      else
	{ char stupidBuffer[1000];
	  fgets(stupidBuffer, 1000, file);
	}
      res = fscanf(file, "%s", lineHeader);
    }
  for(size_t i = 0; i < vertexIndices.size(); i++)
    {
      unsigned int vertexIndex = vertexIndices[i];
      Vector3 vertex = temp_vertices[vertexIndex-1];
      //glm::vec3 glm_v = glm_vertices[vertexIndex-1];
      
      out_vertices.push_back(vertex);
    }
  for(size_t i = 0; i < uvIndices.size(); i++)
    {
      unsigned int uvIndex = uvIndices[i];
      Vector2 uv = temp_uvs[uvIndex-1];
      //glm::vec2 glm_uv = glm_uvs[uvIndex-1];
      //std::cout << "uv " << (()uv)[0] << " glm_uv " << glm::to_string(glm_uv) << std::endl;
      out_uvs.push_back(uv);
    }
  for(size_t i = 0; i < normalIndices.size(); i++)
    {
      unsigned int normalIndex = normalIndices[i];
      Vector3 normal = temp_normals[normalIndex-1];
      out_normals.push_back(normal);
    }
  return true;
}


