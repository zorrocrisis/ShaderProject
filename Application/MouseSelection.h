#pragma once
#define MOUSE_SELECTION

#include <GL/glew.h>;
#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <iostream>

struct MouseButton {
	bool Active; // Flag to check if mouse is being clicked
	double lastPositionX; // Mouse position X
	double lastPositionY; // Mouse position Y
	bool keepDepth; // Should we store information regarding the "depth" of mouse position?
	GLfloat depth; // Used to store said information
};

class MouseSelection
{
private:
	int windowWidth;
	int windowHeight;
	std::map<int, std::string> referenceCorresponce;

public:
	MouseButton LeftMB;
	MouseButton RightMB;

	MouseSelection();
	MouseSelection(int winW, int winH)
	{
		windowHeight = winW;
		windowHeight = winH;
		LeftMB = { false, (double)winW / 2, (double)winH / 2 , false };
		RightMB = { false, (double)winW / 2, (double)winH / 2, false };
	};
	~MouseSelection() {};

	// Stencil
	void ConfigureStencilForSelection();
	void AssignReference(int i, std::string name = "no_name_given");
	std::string RetrieveReference(int i);
	GLuint ReadPixel(int mouseIdent = 0);

	// Updates
	void WindowUpdated(int winW, int winH);
	void MousePositionUpdated(GLFWwindow* win);
	void LeftMBClicked(GLFWwindow* win);
	void RightMBClicked(GLFWwindow* win);
	glm::vec3 MouseDrag(glm::mat4 projMatrix, glm::mat4 vMatrix, int mouseIdent = 0);

	// Resets
	void ResetDepth(int mouseIdent = 0);

};