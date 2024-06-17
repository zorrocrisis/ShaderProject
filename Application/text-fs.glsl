#version 330 core

in vec2 textTexCoords;
out vec4 outColor;

uniform sampler2D textTex; // bitmap
uniform vec3 ColorUniform;

void main(void)
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTex, textTexCoords).r);
	outColor = vec4(ColorUniform, 1.0) * sampled; // alpha lower than 1 to make text transparent

	// If you would like to only view the text quads, uncomment the previous lines (some warnings will occur) and just use:
	//outColor = vec4(ColorUniform, 0.8);


}