#version 300 es

precision highp float;
precision mediump int;
precision lowp sampler2D;

in vec2 UV;

out vec3 color;

uniform sampler2D myTextureSampler;

void main() {
  color = texture(myTextureSampler, UV).rgb;
}
