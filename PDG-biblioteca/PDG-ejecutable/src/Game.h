#pragma once
#include "BaseGame.h"
class Game: BaseGame
{
private:
	Timer* timer;
	Shape* shapeA;
	Shape* cubeA;
	Shape* cubeB;
	Shape* cubeC;
	//test
	Sprite* sprite1;
	Sprite* sprite2;
	TileMap* tileMap;
	Animation* animation;
	Camera* _camera;
	Lightning* _lightA;
	Lightning* _lightB;
	Lightning* _lightC;

	Model* knuckles;
	Model* gun;
	Model* backpack;
	Model* pedrito;

	vector<Plane*> planes;
	vector<Shape*> cornerCube;

	float speedX = 0;
	float speedY = 0;
	float speedZ = 0;
	float growSpeed = 0;
	float rotXSpeed = 0;
	float rotYSpeed = 0;
	float rotZSpeed = 0;

	float camSpeedX = 0.0f;
	float camSpeedY = 0.0f;
	float camSpeedZ = 0.0f;

	float camTargetX = 0.0f;
	float camTargetY = 0.0f;
	float camTargetZ = 0.0f;
	float camRotSpeed = 50.0f;

	bool isPressed[9];

	double mouseLastX;
	double mouseLastY;
	double mouseCurrentX;
	double mouseCurrentY;
	double mousePitch;
	double mouseYaw;
public:
	Game();
	~Game();
	void play();
	void initGame(Renderer* renderer) override;
	void updateGame(CollisionManager collManager, Input* input) override;
	void destroyGame() override;
	void mouse_callback(Window window, Camera camera);
};