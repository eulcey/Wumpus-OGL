#version 330 core

in vec2 UV;
in vec3 normal0;
in vec3 worldPos0;
in vec4 ShadowCoord;
//out vec3 eyeDirection_cameraspace;
//out vec3 normal_cameraspace;

out vec4 color;

uniform sampler2D myTextureSampler;
uniform vec3 gEyeWorldPos;
uniform float gSpecularIntensity;
uniform float gSpecularPower;
uniform sampler2DShadow shadowMap;

struct DirectionalLight
{
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
  vec3 direction;
};

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}
uniform DirectionalLight gDirectionalLight;

void main() {
  vec4 ambientColor = vec4(gDirectionalLight.color, 1.0f) * gDirectionalLight.ambientIntensity;
  
  vec3 lightDirection = -gDirectionalLight.direction;
  vec3 normal = normalize(normal0);
  
  float diffuseFactor = dot(normal, lightDirection);

  vec4 diffuseColor = vec4(0, 0, 0, 0);
  vec4 specularColor = vec4(0, 0, 0, 0);
  
  if(diffuseFactor > 0) {
    diffuseColor = vec4(gDirectionalLight.color, 1.0f) * gDirectionalLight.diffuseIntensity * diffuseFactor;

    vec3 vertexToEye = normalize(gEyeWorldPos - worldPos0);
    vec3 lightReflect = normalize(reflect(-lightDirection, normal));
    float specularFactor = dot(vertexToEye, lightReflect);
    if(specularFactor > 0) {
      specularFactor = pow(specularFactor, gSpecularPower);
      specularColor = vec4(gDirectionalLight.color, 1.0f) * gSpecularIntensity * specularFactor;
    }
  }

	
	float visibility=1.0;

	// Fixed bias, or...
	float bias = 0.005;

	// Sample the shadow map 4 times
	for (int i=0;i<4;i++){
		// use either :
		//  - Always the same samples.
		//    Gives a fixed pattern in the shadow, but no noise
		int index = i;
		//  - A random sample, based on the pixel's screen location. 
		//    No banding, but the shadow moves with the camera, which looks weird.
		// int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
		//  - A random sample, based on the pixel's position in world space.
		//    The position is rounded to the millimeter to avoid too much aliasing
		// int index = int(16.0*random(floor(Position_worldspace.xyz*1000.0), i))%16;
		
		// being fully in the shadow will eat up 4*0.2 = 0.8
		// 0.2 potentially remain, which is quite dark.
		visibility -= 0.2*(1.0-texture( shadowMap, vec3(ShadowCoord.xy + poissonDisk[index]/700.0,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
	}

  color = texture2D(myTextureSampler, UV.xy) * (ambientColor + visibility * diffuseColor + visibility * specularColor);

  //color = vec4(visibility, visibility, visibility, 1);

}
