#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normal;

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec2 UV;
out vec3 normal0;

void main() {
  mat4 MVP = ProjMatrix * ViewMatrix * ModelMatrix;
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

  UV = vertexUV;
  normal0 = ( ModelMatrix * vec4(normal, 0.0)).xyz;
}
