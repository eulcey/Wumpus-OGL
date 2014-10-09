#version 330 core

in vec2 UV;
in vec3 normal0;
in vec3 worldPos0;

out vec4 color;

uniform sampler2D myTextureSampler;
uniform vec3 gEyeWorldPos;
uniform float gSpecularIntensity;
uniform float gSpecularPower;

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
  
  vec3 lightDirection = -gDirectionalLight.direction;
  vec3 normal = normalize(normal0);
  
  float diffuseFactor = dot(normal, lightDirection);

  vec4 diffuseColor = vec4(0, 0, 0, 0);
  vec4 specularColor = vec4(0, 0, 0, 0);
  
  
  if(diffuseFactor > 0.0) {
    diffuseColor = vec4(gDirectionalLight.color, 1.0f) * gDirectionalLight.diffuseIntensity * diffuseFactor;

    vec3 vertexToEye = normalize(gEyeWorldPos - worldPos0);
    vec3 lightReflect = normalize(reflect(gDirectionalLight.direction, normal));
    float specularFactor = dot(vertexToEye, lightReflect);
    specularFactor = pow(specularFactor, gSpecularPower);
    if(specularFactor > 0.0) {
      specularColor = vec4(gDirectionalLight.color, 1.0f) * gSpecularIntensity * specularFactor;
    }
  }

  color = texture2D(myTextureSampler, UV.xy) * (ambientColor + diffuseColor + specularColor);
}
