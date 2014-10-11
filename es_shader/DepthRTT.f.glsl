#version 300 es

precision highp float;
precision mediump int;

// Ouput data
layout(location = 0) out float fragmentdepth;


void main(){
	fragmentdepth = gl_FragCoord.z;
}
