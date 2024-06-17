#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec3 inTexcoord;

out vec3 exPosition;
out vec3 exNormal;
out vec3 exTexcoord;
out vec3 LightDirection;

uniform mat4 ModelMatrix;
uniform vec3 LightPosition;
uniform float SilhouetteOffset;
uniform int Levers[2];
uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};


void main()
{

	// This will allow the shading to change with model movement
	exNormal = normalize( ModelMatrix * vec4(inNormal, 0.0) ).xyz;
	exPosition = inPosition;
	exTexcoord = inTexcoord;

    // LightDirection
    LightDirection = normalize(LightPosition.xyz - exPosition.xyz);

	// Calculate normal matrix in order to perform silhouette offset
	mat3 NormalMatrix = mat3(transpose(inverse(ModelMatrix)));
	vec3 offset = SilhouetteOffset * normalize(vec3(NormalMatrix * inNormal)); // object offset at each point

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4((inPosition + offset), 1.0);
}