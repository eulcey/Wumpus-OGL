#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normal;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 ShadowCoord;

// Values that stay constant for the whole mesh.

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform mat4 depthBiasMVP;


void main(){

	// Output position of the vertex, in clip space : MVP * position
	mat4 MVP = ProjMatrix * ViewMatrix * ModelMatrix;
	//gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	gl_Position = depthBiasMVP * vec4(vertexPosition_modelspace,1);
	
	ShadowCoord = depthBiasMVP * vec4(vertexPosition_modelspace,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}
