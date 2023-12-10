#pragma once
#include "BaseGame.h"

class Game: BaseGame
{
private:
	Timer* _timer;
	Camera* _camera;

	Lightning* _lightA;
	Lightning* _lightB;
	Lightning* _lightC;

	Model* _customCharacter;

	vector<Plane*> _planes;
	vector<Shape*> _cornerCube;

	float _speedX = 0;
	float _speedY = 0;
	float _speedZ = 0;
	float _growSpeed = 0;
	float _rotXSpeed = 0;
	float _rotYSpeed = 0;
	float _rotZSpeed = 0;

	float _camSpeedX = 0.0f;
	float _camSpeedY = 0.0f;
	float _camSpeedZ = 0.0f;

	const float _camSpeed = 2.0f;

public:
	Game();
	~Game();
	void play();
	void initGame(Renderer* renderer) override;
	void updateGame(CollisionManager collManager) override;
	void destroyGame() override;
};