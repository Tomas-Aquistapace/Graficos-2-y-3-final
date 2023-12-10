#pragma once
#include "Tile.h"
#include "Exports.h"
#include "Shape.h"
#include "CollisionManager.h"

#include <vector>

class AQUISTAPACE_API TileMap: public Entity
{
private:
	Texture* textureUsed;
	unsigned int texture;
	vector<Tile*> tiles;
	vector<Tile*> tilesLayout;
	float _tileWidth;
	float _tileHeight;
	float tilemapWidth;
	float tilemapHeight;
	int _imageHeight;
	int _imageWidth;
	bool correctlySet;
	CollisionManager collMan;
	bool collidesWithTileMap(Shape* shape);

public:
	TileMap(Renderer* renderer,int rows, int columns, const char* path, int imageHeight, int imageWidth, float tileWidth, float tileHeight);
	~TileMap();
	void setTileMap(int column, int row, vector<int> tilesId);
	void setTileMap(int column, int row, vector<int> tilesId, vector<bool> tilesWalkable);
	void drawTileMap();
	bool checkCollisionWithTileMap(Shape* sprite, vec3 movement);
};