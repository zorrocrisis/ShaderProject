#version 330 core

out vec4 outColor;

uniform vec3 ColorUniform;

void main(void)
{
	outColor = vec4(ColorUniform, 0.3); // alpha lower than 1 to make text transparent
}