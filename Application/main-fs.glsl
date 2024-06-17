#version 330 core

in vec3 exPosition;
in vec3 exNormal;
in vec3 exTexcoord;
in vec3 LightDirection;

out vec4 FragmentColor;

uniform sampler3D NoiseTexture;
uniform int NeedsNoise;

uniform vec3 ColorUniform;

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
 };

 uniform float SilhouetteOffset;

 uniform bool NeedsHighlight;

 uniform float Levers[2];

void main() 
{ 

	vec3 color = ColorUniform;

	// Ambient and diffuse factors
	float ambient = Levers[1];
	float diffuse = 1.0 - ambient;

	vec3 wcPosition = vec3(ModelMatrix * vec4(exPosition, 1.0));
	vec3 P = wcPosition * 0.5 + 0.5;

    // Normalize the normal, position and light direction
    vec3 Norm = normalize(exNormal);
    vec3 Pos = normalize(-exPosition);
    vec3 Li = normalize(LightDirection);

	// Calculate auxiliary values
	float Kd = max(dot(Li, Norm), 0.0);
	float gradient = fwidth(Norm.x + Norm.y + Norm.z); // If gradient is too low, we add black edges  // HOW TO CALCULATE GRADIENS FOR EDGES=

	// Change diffuse factor
	if(Kd > 0.3)
	{
		diffuse = diffuse;
	}
	else if(Kd > 0.01)
	{
		diffuse = 0.6*diffuse;
	}
	else
	{
		diffuse = 0*diffuse;
	}
	
	float intensity = Kd * diffuse + ambient;

	// Cel shading step function
	if (intensity > 0.75)
	{
		color = vec3(ColorUniform.x, ColorUniform.y, ColorUniform.z);
	}
	else if (intensity > 0.5)
	{
		color = vec3(ColorUniform.x*3.0/4.0, ColorUniform.y*3.0/4.0, ColorUniform.z*3.0/4.0);
	}
	else if (intensity > 0.25)
	{
		color = vec3(ColorUniform.x*1.0/2.0, ColorUniform.y*1.0/2.0, ColorUniform.z*1.0/2.0);
	}
	else
	{
		color = vec3(ColorUniform.x*1.0/4.0, ColorUniform.y*1.0/4.0, ColorUniform.z*1.0/4.0);
	}

	// If object needs highlight, mix between its colour and green
	if(NeedsHighlight)
	{
		color = mix(color, vec3(0.0, 0.8, 0.0), 0.5);
	}

	if (NeedsNoise == 1)
	{	
		float cellSize = 0.65;

		if (Levers[0] == 0) // Example 1 - more stylized (less but bigger black fissures) - default seeing as though this is cel shading!
		{
			float noise = texture(NoiseTexture, exTexcoord).r * 0.5 + 0.5;
			noise = floor(noise/cellSize);
			FragmentColor = vec4(color * intensity * noise, 1.0);
		}
		else if(Levers[0] == 1) // Example 2 - small grainy cork
		{
			float noise = texture(NoiseTexture, exPosition).r * 0.5 + 0.5;
			FragmentColor = vec4(color * intensity * noise, 1.0);
		}
		else if(Levers[0] == 2) // Example 3 - only using the texture (a bit blurry)
		{
			FragmentColor = texture(NoiseTexture, exTexcoord) * vec4(color * intensity, 1.0);
		}
		else if(Levers[0] == 3) // Example 4 - more stylized (more but smaller black fissures)
		{
			float noise = texture(NoiseTexture, exPosition).r * 0.5 + 0.5;
			noise = floor(noise/cellSize);
			FragmentColor = vec4(color * intensity * noise, 1.0);
		}

	}
	else
		FragmentColor = vec4(intensity * color, 1.0);
} 
