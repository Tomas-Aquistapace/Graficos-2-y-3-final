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
	Renderer* renderer;
	CollisionManager collManager;
	Input* input;
	bool gameShouldClose;

protected:
	Window* window;

public:
	BaseGame();
	~BaseGame();
	void initBaseGame(int screenWidth, int screenHeight, const char* title);
	int engineLoop();
	virtual void initGame(Renderer* renderer) = 0;
	virtual void updateGame(CollisionManager collManager,Input* input) = 0;
	virtual void destroyGame() = 0;
};