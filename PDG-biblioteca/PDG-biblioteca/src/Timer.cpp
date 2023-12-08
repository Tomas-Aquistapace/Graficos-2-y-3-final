#include "Timer.h"
Timer::Timer()
{
	time = 0.0f;
	dTime = 0.0f;
	timeBeforeStarting = 0.0f;
	lastFrameTime = 0.0f;
}

void Timer::start()
{
	timeBeforeStarting = glfwGetTime();
}

void Timer::updateTimer()
{
	lastFrameTime = time;
	time = glfwGetTime() - timeBeforeStarting;
	dTime = time - lastFrameTime;
}

float Timer::getTime()
{
	return time;
}

float Timer::getDT()
{
	return dTime;
}