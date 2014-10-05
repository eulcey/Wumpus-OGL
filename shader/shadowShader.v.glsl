#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normal;

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform mat4 depthBiasMVP;

out vec2 UV;
out vec3 normal0;
out vec3 worldPos0;
out vec4 ShadowCoord;
out vec3 eyeDirection_cameraspace;
out vec3 normal_cameraspace;

void main() {
  mat4 MVP = ProjMatrix * ViewMatrix * ModelMatrix;
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);

  ShadowCoord = depthBiasMVP * vec4(vertexPosition_modelspace,1);

  UV = vertexUV;
  normal0 = ( ModelMatrix * vec4(normal, 0.0)).xyz;
  worldPos0 = (ModelMatrix * vec4(vertexPosition_modelspace, 1.0)).xyz;


  // Vector that goes from the vertex to the camera, in camera space.
  // In camera space, the camera is at the origin (0,0,0).
  //eyeDirection_cameraspace = vec3(0,0,0) - ( ViewMatrix * ModelMatrix * vec4(vertexPosition_modelspace,1)).xyz;

  // Normal of the the vertex, in camera space
  //normal_cameraspace = ( ViewMatrix * inverse(transpose(ModelMatrix)) * 
  //  vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.


  // Vector that goes from the vertex to the light, in camera space
  // LightDirection_cameraspace = (V*vec4(LightInvDirection_worldspace,0)).xyz;
}
