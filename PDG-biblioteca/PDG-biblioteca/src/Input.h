#pragma once
#include "Exports.h"
#include "Window.h"
#include "GLFW/glfw3.h"

class SABASAENGINE_API Input
{
	Window* window;
public:
	Input(Window* _window);
	~Input();
	bool isKeyDown(int key);
	bool isKeyUp(int key);
	bool isKeyPressed(int key, bool &pressed);
};

