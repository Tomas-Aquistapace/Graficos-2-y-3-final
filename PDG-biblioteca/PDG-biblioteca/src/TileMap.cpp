#include "TileMap.h"
#include <algorithm>

TileMap::TileMap(Renderer* renderer, int rows, int columns, const char* path, int imageHeight, int imageWidth, float tileScaleX, float tileScaleY):Entity(renderer)
{
	correctlySet = false;
	_imageHeight = imageHeight;
	_imageWidth = imageWidth;
	int tileProportionalWidth = _imageWidth/ columns;
	int tileProportionalHeight = _imageHeight / rows;  // 256 / 16 = 16 / 1
	textureUsed = new Texture(path);
	textureUsed->Bind(0);
	texture = textureUsed->getTex();
	tiles.clear();
	float tileX = 0.0f;
	float tileY = imageHeight;
	int id = 0; //el del profe empieza en 1 pero yo lo puse en 0
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			Tile* newTile = new Tile(renderer, texture, true, id, (float)rows/imageWidth * tileScaleX, (float)columns/imageHeight * tileScaleY);

			//newTile->width()//0.5  256   0.5 / 16 = 0.03125  
				//  0==0				0.5==256
				//	0==0				0.5==256
			newTile->setTextureCoordinates(
			//	0, 1 // 0, 0
			//	0, 1-0.0625f, //0 , 1
			//	0.0625f, 1-0.0625f, // 1, 1
			//	0.0625f, 1, // 1, 0
			//);
				//0.5, 0.5, 0.5, 0, 0, 0, 0, 0.5);

			(tileX + tileProportionalWidth) / _imageWidth, tileY / _imageHeight,
			(tileX + tileProportionalWidth) / _imageWidth, (tileY - tileProportionalHeight) / _imageHeight,
			tileX / _imageWidth, (tileY - tileProportionalHeight) / _imageHeight,
			tileX / _imageWidth, tileY / _imageHeight

////         0                   0
//////		0			,		0
///*0*/				tileX / _imageWidth, tileY / _imageHeight,
////         0                       1
//////		0			,		0,0625
///*1*/				tileX / _imageWidth, (tileY - tileProportionalHeight) / _imageHeight,
//// 1                                        1
////(0    +	16)		/  256 = 0,0625, ( 0   +  16   ) / 256 = 0.0625
///*2*/			(tileX + tileProportionalWidth) / _imageWidth, (tileY - tileProportionalHeight) / _imageHeight,
////     1                                     0
//////			( 0,0625)			,		0 
///*3*/			(tileX + tileProportionalWidth) / _imageWidth, tileY / _imageHeight
			);

			tileX += tileProportionalWidth;
			tiles.push_back(newTile);
			
			
			id++;
		}
		tileX = 0;
		tileY -= tileProportionalHeight;
	}
	_tileWidth = tiles[0]->width;
	_tileHeight = tiles[0]->height;
}
void TileMap::setTileMap(int column, int row, vector<int> tilesId)
{
	tilesLayout.clear();
	int tilesAmount = row * column;
	if (tilesId.size() >= tilesAmount)
	{
		correctlySet = true;
	}
	else
	{
		correctlySet = false;
		cout << "Tilemap not set correctly"<<endl;
		return;
	}
	vector<int>::iterator it = tilesId.begin();
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			if (find(tilesId.begin(), it, tilesId[i * column + j]) == it)
				tilesLayout.push_back(tiles[tilesId[i * column + j]]);
			else
				tilesLayout.push_back(tiles[tilesId[i * column + j]]->clone());
			it++;
		}
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			tilesLayout[i*column + j]->setPosition(vec3(j*tilesLayout[i*column + j]->width,i*-tilesLayout[i*column + j]->height,0) - vec3(column / 2.0f * tilesLayout[i*column + j]->width, (row / 2.0f * tilesLayout[i*column + j]->height) - tilesLayout[i*column + j]->height/2,0));
		}
	}
	tilemapWidth = (tilesLayout[tilesLayout.size()-1]->getPosition().x + tilesLayout[tilesLayout.size() - 1]->width/2.0f) - (tilesLayout[0]->getPosition().x - tilesLayout[0]->width/2.0f);
	tilemapHeight = (tilesLayout[0]->getPosition().y + tilesLayout[0]->height / 2.0f) - (tilesLayout[tilesLayout.size() - 1]->getPosition().y - tilesLayout[tilesLayout.size() - 1]->height / 2.0f);
}

void TileMap::setTileMap(int column, int row, vector<int> tilesId, vector<bool> tilesWalkable)
{
	tilesLayout.clear();
	int tilesAmount = row * column;
	if (tilesId.size() >= tilesAmount && tilesWalkable.size() >= tilesAmount)
	{
		correctlySet = true;
	}
	else
	{
		correctlySet = false;
		cout << "Tilemap not set correctly" << endl;
		return;
	}
	vector<int>::iterator it = tilesId.begin();
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			if (find(tilesId.begin(), it, tilesId[i * column + j]) == it)
				tilesLayout.push_back(tiles[tilesId[i * column + j]]);
			else
				tilesLayout.push_back(tiles[tilesId[i * column + j]]->clone());
			it++;
		}
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			tilesLayout[i*column + j]->setPosition(vec3(j*tilesLayout[i*column + j]->width, i*-tilesLayout[i*column + j]->height, 0) - vec3(column / 2.0f * tilesLayout[i*column + j]->width, (row / 2.0f * tilesLayout[i*column + j]->height) - tilesLayout[i*column + j]->height / 2, 0));
			tilesLayout[i*column + j]->setWalkable(tilesWalkable[i*column + j]);
		}
	}
	tilemapWidth = (tilesLayout[tilesLayout.size() - 1]->getPosition().x + tilesLayout[tilesLayout.size() - 1]->width / 2.0f) - (tilesLayout[0]->getPosition().x - tilesLayout[0]->width / 2.0f);
	tilemapHeight = (tilesLayout[0]->getPosition().y + tilesLayout[0]->height / 2.0f) - (tilesLayout[tilesLayout.size() - 1]->getPosition().y - tilesLayout[tilesLayout.size() - 1]->height / 2.0f);
}

TileMap::~TileMap()
{
	if (textureUsed) delete textureUsed;
	tiles.clear();
	tilesLayout.clear();
}

void TileMap::drawTileMap()
{
	if (!correctlySet) return;

	for (int i = 0; i < tilesLayout.size(); i++)
	{
		tilesLayout[i]->draw();
	}
}

//hacer que haga el checkeo de colisiones no a la fuerza bruta sino que checkee nada mas
//los tiles con los que puede estar colisionando transformando la posicion a coordenadas de tile
//ADEMAS LA COLISION TIENE QUE FUNCIONAR SIN IMPORTAR LA ESCALA, ES UN PROBLEMA DE LA COLISION EN
//GENERAL, NO SOLO TILEMAP, PUEDE TENER QUE VER CON CHECKEAR LA "NO-COLISION" EN VEZ DE LA COLISION
//ARREGLAR BIEN EL DIBUJADO, QUEDO LO DE LOS TRIANGULOS SIN IMPLEMENTAR
bool TileMap::checkCollisionWithTileMap(Shape* shape, vec3 movement)
{
	if (!collidesWithTileMap(shape))
		return false;
	
	bool collides = false;
	
	//float convertedPosX = (shape->getPosition().x + (tilemapWidth/2.0f))   * _tileWidth;
	//float convertedPosY = (shape->getPosition().y - (tilemapHeight / 2.0f))  * _tileHeight;

	for (int i = 0; i < tilesLayout.size(); i++)
	{
		if (collMan.CheckCollision(tilesLayout[i], shape))
		{
			cout << "Colisiona con el tile " << i << endl;
			collides = true;
			if (!tilesLayout[i]->_isWalkable)
			{
				shape->setPosition(shape->getPosition() - movement);
			}
		}
	}
	return collides;

	//cout << getPosition().x-tilemapWidth/2<< endl;
	//cout << getPosition().y-tilemapHeight/2<< endl;
	//cout << getPosition().x + tilemapWidth / 2 << endl;
	//cout << getPosition().y + tilemapHeight / 2 << endl;
	//cout << "height"<<tilemapHeight<<endl;
	//cin.get();
	//cout << shape->getPosition().y << endl;
	//								0.5				*  0.0625	
	//
	//int leftTile = convertedPosX / _tileWidth;
	//int rightTile = (convertedPosX + shape->width) / _tileWidth;
	//
	//int topTile = (convertedPosY / _tileHeight) * -1;
	//int bottomTile = (convertedPosY + shape->height) / _tileHeight;
	//
	//cout << leftTile  <<endl;
	//cout << rightTile <<endl;
	//cout << topTile   <<endl;
	//cout << bottomTile<<endl;
}

bool TileMap::collidesWithTileMap(Shape* shape)
{
													//ARREGLAR ESTO QUE POR ALGUNA RAZON ACA ES SSIN EL /2.0f
	if (shape->getPosition().x + shape->width / 2.0f <	getPosition().x - tilemapWidth ||
		shape->getPosition().x - shape->width / 2.0f > getPosition().x + tilemapWidth / 2.0f ||
		shape->getPosition().y + shape->height / 2.0f < getPosition().y - tilemapHeight / 2.0f ||
		shape->getPosition().y - shape->height / 2.0f > getPosition().y + tilemapHeight / 2.0f)
		return false;
	else return true;
}