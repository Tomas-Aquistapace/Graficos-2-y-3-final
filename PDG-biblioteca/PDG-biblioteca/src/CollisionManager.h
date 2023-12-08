#pragma once
#include "Exports.h"
#include "Shape.h"
#include "Sprite.h"
#include "Tile.h"
#include "glm/glm.hpp"

struct Collider
{
	float width;
	float height;
	vec3 pos;
	vec3 scale;
};

struct CollisionResult
{
	bool left;
	bool right;
	bool top;
	bool bottom;
};

class SABASAENGINE_API CollisionManager
{
	Collider a;
	Collider b;
	CollisionResult result;
	bool isColliding();
public:
	bool CheckCollision(Shape* shapeA, Shape* ShapeB);
	bool CheckCollision(Shape* shape, Sprite* sprite);
	bool CheckCollision(Sprite* spriteA, Sprite* spriteB);
	bool CheckCollision(Tile* tile, Shape* shape);
	bool CheckCollisionAndPush(Shape* shape, Sprite* sprite, vec3 movement);
	bool CheckCollisionAgainstStatic(Shape* shape, Sprite* sprite, vec3 movement);
	bool CheckCollisionAgainstStatic(Shape* shape, Tile* tile, vec3 movement);
};