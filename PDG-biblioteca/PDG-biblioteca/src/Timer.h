#pragma once
#include "Exports.h"
#include "GLFW/glfw3.h"
class SABASAENGINE_API Timer
{
private:
	float timeBeforeStarting;
	float lastFrameTime;
	float time;
	float dTime;
public:
	Timer();
	void start();
	void updateTimer();
	float getTime();
	float getDT();
};