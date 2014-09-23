#version 330 core

in vec2 UV;
in vec3 normal0;

out vec4 color;

uniform sampler2D myTextureSampler;

struct DirectionalLight
{
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
  vec3 direction;
};

uniform DirectionalLight gDirectionalLight;

void main() {
  vec4 ambientColor = vec4(gDirectionalLight.color, 1.0f) * gDirectionalLight.ambientIntensity;
  float diffuseFactor = dot(normalize(normal0), -gDirectionalLight.direction);

  vec4 diffuseColor;
  if(diffuseFactor > 0) {
    diffuseColor = vec4(gDirectionalLight.color, 1.0f) * gDirectionalLight.diffuseIntensity * diffuseFactor;
  } else {
    diffuseColor = vec4(0, 0, 0, 0);
  }

  color = texture2D(myTextureSampler, UV.xy) * (ambientColor + diffuseColor);
}
