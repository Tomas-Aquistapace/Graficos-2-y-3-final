#pragma once

#include "Exports.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class AQUISTAPACE_API Window
{
public:
	void createWindow(int screenWidth, int screenHeight, const char* title);
	GLFWwindow* getWindow();
	bool checkWindowCreation();
private:
	GLFWwindow* glfwWindow;
};
