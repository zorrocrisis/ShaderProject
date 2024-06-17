#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/gtx/quaternion.hpp>

#include "mgl.hpp"
#include <assimp/Importer.hpp>
#include "node.h"
#include <array>
#include "mglSampler.hpp"
#include "mglTexture.hpp"

struct RotationAngles {
	float xAngle;
	float yAngle;
	float zAngle;
};


class node
{
public:
	const GLuint UBO_BP1 = 0;
	//program shader
	mgl::ShaderProgram* Shaders = nullptr;

	//Mesh
	GLint ModelMatrixId=0;
	mgl::Mesh* Mesh = nullptr;
	
	//ModelMatrix
	glm::mat4 Model;
	glm::mat4 NormalMatrix;
	
	glm::vec3 Translation;  /* Translation relative to parent.*/ 
	glm::quat Rotation;  /* Rotation relative to parent. */
	glm::vec3 Scale;  /* Scale relative to parent. */

	glm::vec3 Color;

	mgl::Texture3D* Texture;
	mgl::TextureInfo* TextInfo;
	bool NodeNeedsTexture;
	bool NodeNeedsHighlight;

	RotationAngles theseRotationAngles;

	node* next_level;   /* Link to object at next level. */

	node();

	// mesh, shaders, modelID, translation, rotation, scale, colour, light position, texture and need of texture
	node(mgl::Mesh* mesh, mgl::ShaderProgram* shaders, GLint Id, glm::vec3 t,glm::quat q,glm::vec3 s,glm::vec3 c, int y_n, mgl::Texture3D* tex, mgl::TextureInfo* ti) {
		
		Mesh = mesh;
		Shaders = shaders;
		ModelMatrixId = Id;
		Translation = t;
		Rotation =	q;
		Scale = s;
		Color = c;
		Texture = tex;
		TextInfo = ti;
		NodeNeedsTexture = y_n;
		theseRotationAngles = { 0.0f, 0.0f, 0.0f };
	}

	void linkChild(node* next_lvl);

	void initializeModels();
	void rotateModel(glm::quat quat);
	void translateModel(glm::vec3 translat);
	void updateModel();
	void addYRotation(float deltaAngle);
	void addXRotation(float deltaAngle);
	void addZRotation(float deltaAngle);

	void drawCelSelf();
};

