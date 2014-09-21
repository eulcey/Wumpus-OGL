#version 330 core
layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec3 fragmentColor;

void main() {
  mat4 MVP = ProjMatrix * ViewMatrix * ModelMatrix;
  gl_Position = MVP * vertexPosition_modelspace;
  fragmentColor = vertexColor.xyz;
}