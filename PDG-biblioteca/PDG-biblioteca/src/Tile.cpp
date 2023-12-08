#include "Tile.h"

Tile::Tile(Renderer* renderer, unsigned int newTexture, bool transparency, int id, float scaleX, float scaleY):Sprite(renderer, newTexture, transparency,scaleX,scaleY)
{
	_id = id;
	_isWalkable = true;
}

void Tile::setWalkable(bool isWalkable)
{
	_isWalkable = isWalkable;
}

Tile* Tile::clone()
{
	Tile* clonedTile = new Tile(rend, texture, _transparency, _id, _scaleX, _scaleY);

	unsigned int uStart = 3;
	unsigned int vStart = 4;
	unsigned int stride = rend->getAttribElementsAmount();
	unsigned int vertexNum = 0;
	float texCoordinates[8];
	for (int i = 0; i < 8; i++)
	{
		if (i%2==0)
			texCoordinates[i] = verticesData[stride*vertexNum + uStart];
		else
		{
			texCoordinates[i] = verticesData[stride*vertexNum + vStart];
			vertexNum++;
		}
	}

	clonedTile->setTextureCoordinates(texCoordinates[0], texCoordinates[1], texCoordinates[2], texCoordinates[3], texCoordinates[4], texCoordinates[5], texCoordinates[6], texCoordinates[7]);

	return clonedTile;
}

Tile::~Tile()
{

}