#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 ShadowCoord;

// Ouput data
//layout(location = 0) out vec3 color;
layout(location = 0) out float fragmentdepth;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform sampler2DShadow shadowMap;

void main(){

	// Light emission properties
	vec3 LightColor = vec3(1,1,1);
	
	// Material properties
	vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;

	float visibility = texture( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w));

	vec3 color = visibility * MaterialDiffuseColor * LightColor;

	if(visibility > 0)
		color = vec3(1, 0, 0);
	if(visibility > 0.3)
		color = vec3(0, 1, 0);
	if(visibility == 0)
		color = vec3(0, 0, 1);

	fragmentdepth = gl_FragCoord.z;
}
