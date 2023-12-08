#include "CollisionManager.h"

bool CollisionManager::isColliding()
{
	bool collisionX = true;
	bool collisionY = true;
	result.bottom = true;
	result.top = true;
	result.left = true;
	result.right = true;

	if (((a.pos.x + a.width*a.scale.x / 2) < (b.pos.x - b.width*b.scale.x / 2)
		|| (a.pos.x + a.width*a.scale.x / 2) > (b.pos.x + b.width*b.scale.x / 2))
		)
	{
		result.left = false;
	}
	
	if (((a.pos.x - a.width*a.scale.x / 2) < (b.pos.x - b.width*b.scale.x / 2)
		|| (a.pos.x - a.width*a.scale.x / 2) > (b.pos.x + b.width*b.scale.x / 2)))
	{
		result.right = false;
	}

	collisionX = (result.left || result.right);

	if (((a.pos.y + a.height*a.scale.y / 2) < (b.pos.y - b.height*b.scale.y / 2)
		|| (a.pos.y + a.height*a.scale.y / 2) > (b.pos.y + b.height*b.scale.y / 2))
		)
	{
		result.bottom = false;
	}
	if ((a.pos.y - a.height*a.scale.y / 2) < (b.pos.y - b.height*b.scale.y / 2)
		|| (a.pos.y - a.height*a.scale.y / 2) > (b.pos.y + b.height*b.scale.y / 2))
	{
		result.top = false;
	}
	
	collisionY = (result.bottom || result.top);

	return (collisionX && collisionY);
}

bool CollisionManager::CheckCollision(Shape* shapeA, Shape* ShapeB)
{
	a.width = shapeA->width;
	a.height = shapeA->height;
	a.pos = shapeA->getPosition();
	a.scale = shapeA->getScale();

	b.width = ShapeB->width;
	b.height = ShapeB->height;
	b.pos = ShapeB->getPosition();
	b.scale = ShapeB->getScale();

	return isColliding();
}

bool CollisionManager::CheckCollision(Shape* shape, Sprite* sprite)
{
	a.width = shape->width;
	a.height = shape->height;
	a.pos = shape->getPosition();
	a.scale = shape->getScale();

	b.width = sprite->width;
	b.height = sprite->height;
	b.pos = sprite->getPosition();
	b.scale = sprite->getScale();

	return isColliding();
}

bool CollisionManager::CheckCollision(Sprite* spriteA, Sprite* spriteB)
{
	a.width = spriteA->width;
	a.height = spriteA->height;
	a.pos = spriteA->getPosition();
	a.scale = spriteA->getScale();

	b.width = spriteB->width;
	b.height = spriteB->height;
	b.pos = spriteB->getPosition();
	b.scale = spriteB->getScale();

	return isColliding();
}

bool CollisionManager::CheckCollision(Tile* tile, Shape* shape)
{
	a.width = tile->width;
	a.height = tile->height;
	a.pos = tile->getPosition();
	a.scale = tile->getScale();

	b.width = shape->width;
	b.height = shape->height;
	b.pos = shape->getPosition();
	b.scale = shape->getScale();

	return isColliding();
}

bool CollisionManager::CheckCollisionAndPush(Shape* shape, Sprite* sprite, vec3 movement)
{
	a.width = shape->width;
	a.height = shape->height;
	a.pos = shape->getPosition();
	a.scale = shape->getScale();

	b.width = sprite->width;
	b.height = sprite->height;
	b.pos = sprite->getPosition();
	b.scale = sprite->getScale();

	if (isColliding())
	{
		shape->setPosition(shape->getPosition() - movement / 2.0f);
		sprite->setPosition(sprite->getPosition() + movement / 2.0f);
		return true;
	}
}

bool CollisionManager::CheckCollisionAgainstStatic(Shape* shape, Sprite* sprite, vec3 movement)
{
	a.width = shape->width;
	a.height = shape->height;
	a.pos = shape->getPosition();
	a.scale = shape->getScale();

	b.width = sprite->width;
	b.height = sprite->height;
	b.pos = sprite->getPosition();
	b.scale = sprite->getScale();

	if (isColliding())
	{
		shape->setPosition(shape->getPosition() - movement);
		return true;
	}
}

bool CollisionManager::CheckCollisionAgainstStatic(Shape* shape, Tile* tile, vec3 movement)
{
	a.width = shape->width;
	a.height = shape->height;
	a.pos = shape->getPosition();
	a.scale = shape->getScale();

	b.width = tile->width;
	b.height = tile->height;
	b.pos = tile->getPosition();
	b.scale = tile->getScale();

	if (isColliding())
	{
		shape->setPosition(shape->getPosition() - movement);
		return true;
	}
}

