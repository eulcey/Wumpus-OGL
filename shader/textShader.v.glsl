#version 330 core

layout(location = 0) in vec2 vertexPosition_screenspace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

void main() {
  vec2 vertPos_homoneneousspace = vertexPosition_screenspace - vec2(400, 300);
  vertPos_homoneneousspace /= vec2(400, 300);
  gl_Position = vec4(vertPos_homoneneousspace, 0, 1);

  UV = vertexUV;
}
