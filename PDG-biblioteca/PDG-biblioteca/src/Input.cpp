#include "Input.h"

Input::Input(Window* _window)
{
	window = _window;
}

Input::~Input()
{

}

bool Input::isKeyDown(int key)
{
	return (glfwGetKey(window->getWindow(), key) == GLFW_PRESS);
}

bool Input::isKeyUp(int key)
{
	return (glfwGetKey(window->getWindow(), key) == GLFW_RELEASE);
}

bool Input::isKeyPressed(int key, bool &pressed)
{
	if (!pressed)
	{
		if (isKeyDown(key))
		{
			pressed = true;
			return true;
		}
	}
	else
	{
		if (isKeyUp(key))
		{
			pressed = false;
		}
		return false;
	}
}