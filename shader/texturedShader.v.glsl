#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec2 UV;

void main() {
  mat4 MVP = ProjMatrix * ViewMatrix * ModelMatrix;
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

  UV = vertexUV;
}