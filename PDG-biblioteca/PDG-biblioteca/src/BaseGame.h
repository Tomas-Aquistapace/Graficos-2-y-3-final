#pragma once
#include "Exports.h"
#include "Renderer.h"
#include "Timer.h"
#include "CollisionManager.h"
#include "Input.h"
#include "Shape.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Model.h"
#include "Camera.h"
#include "Lightning.h"

class AQUISTAPACE_API BaseGame
{
private:
	Renderer* _renderer;
	CollisionManager _collManager;
	bool _gameShouldClose;

protected:
	Window* _window;

public:
	BaseGame();
	~BaseGame();
	void initBaseGame(int screenWidth, int screenHeight, const char* title);
	int engineLoop(float r, float g, float b, float a);
	virtual void initGame(Renderer* renderer) = 0;
	virtual void updateGame(CollisionManager collManager) = 0;
	virtual void destroyGame() = 0;

	void exitApplication();

	void activateFPSCamera(Camera* camera, float sensitivity);
	void deactivateFPSCamera();
};