#include "Window.h"
#include <iostream>
using namespace std;

void Window::createWindow(int screenWidth, int screenHeight, const char* title)
{
	glfwWindow = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
}

GLFWwindow* Window::getWindow()
{
	return glfwWindow;
}

bool Window::checkWindowCreation()
{
	return (glfwWindow);
}