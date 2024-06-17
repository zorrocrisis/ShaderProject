#version 330 core

in vec4 inVertex; // vec4(vec2(position), vec2(texture))
out vec2 textTexCoords;

uniform mat4 OrthoProjection;

void main()
{
	// We want to orthographically project the text on the screen
	gl_Position = OrthoProjection * vec4(inVertex.xy, 0.0, 1.0);

	textTexCoords = inVertex.zw;
}
