#pragma once
#include "Exports.h"
#include "Sprite.h"

class AQUISTAPACE_API Tile: public Sprite
{
	int _id;
public:
	Tile(Renderer* renderer, unsigned int newTexture, bool transparency, int id, float width, float heigth);
	~Tile();
	bool _isWalkable;
	void setWalkable(bool isWalkable);
	Tile* clone();
};
