#include "Sprite.h"
#include "glm/gtc/type_ptr.hpp"

Sprite::Sprite(Renderer* renderer, const char* path,bool transparency) :Entity(renderer)
{
	rend = renderer;
	initSprite(path);
	_transparency = transparency;
	textureUsed = NULL;
}
Sprite::Sprite(Renderer* renderer, unsigned int newTexture, bool transparency) :Entity(renderer)
{
	rend = renderer;
	initSprite(newTexture);
	_transparency = transparency;
	textureUsed = NULL;
}
Sprite::Sprite(Renderer* renderer, unsigned int newTexture, bool transparency, float newWidth, float newHeight) :Entity(renderer)
{
	rend = renderer;
	initSprite(newTexture,newWidth,newHeight);
	_transparency = transparency;
	textureUsed = NULL;
}
Sprite::~Sprite()
{
	if (textureUsed) delete textureUsed;
}
void Sprite::initSprite(const char* path)
{
	width =  baseWidth;
	height = baseHeight;
	rend->creatoVAO(vao);
	rend->createVBO(verticesData, vertexAmount, vbo);
	rend->createEBO(indicesData, indexAmount, ebo);
	textureUsed = new Texture(path);
	textureUsed->Bind(0);
	texture = textureUsed->getTex();
	rend->setTexture(texture);
}
void Sprite::initSprite(unsigned int newTexture)
{
	width = baseWidth;
	height = baseHeight;
	rend->creatoVAO(vao);
	rend->createVBO(verticesData, vertexAmount, vbo);
	rend->createEBO(indicesData, indexAmount, ebo);
	texture = newTexture;
	rend->setTexture(texture);
}
void Sprite::initSprite(unsigned int newTexture, float scaleX, float scaleY)
{
	_scaleX = scaleX;
	_scaleY = scaleY;


	unsigned int stride = rend->getAttribElementsAmount();

	for (int i = 0; i < 4; i++)
	{
		verticesData[i * stride] = (sign(verticesData[i * stride]) * (baseWidth / 2)) * _scaleX;
		verticesData[i * stride +1] = (sign(verticesData[i * stride +1]) * (baseWidth / 2)) * _scaleX;
	}

	width =  verticesData[0] - verticesData[stride*2];
	height = verticesData[1] - verticesData[stride * 2 + 1];

	rend->creatoVAO(vao);
	rend->createVBO(verticesData, vertexAmount, vbo);
	rend->createEBO(indicesData, indexAmount, ebo);
	texture = newTexture;
	rend->setTexture(texture);
}

void Sprite::updateSprite(Timer & timer) {
	if (!_animation) {
		return;
	}
	_animation->update(timer);
	_currentFrame = _animation->getCurrentFrame();
	if (_currentFrame != _prevFrame
		|| _currentFrame == _animation->getAnimation().size()-1
		|| _currentFrame == 0) {
		setTextureCoordinates(
			_animation->getAnimation()[_currentFrame].coordinates[1].U, _animation->getAnimation()[_currentFrame].coordinates[1].V,
			_animation->getAnimation()[_currentFrame].coordinates[2].U, _animation->getAnimation()[_currentFrame].coordinates[2].V,
			_animation->getAnimation()[_currentFrame].coordinates[3].U, _animation->getAnimation()[_currentFrame].coordinates[3].V,
			_animation->getAnimation()[_currentFrame].coordinates[0].U, _animation->getAnimation()[_currentFrame].coordinates[0].V
		);
		_prevFrame = _currentFrame;
	}
	setAnimation(_animation);
}

void Sprite::setTextureCoordinates(float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3)
{
	unsigned int uStart = 3;
	unsigned int vStart = 4;
	unsigned int stride = rend->getAttribElementsAmount();
	float uElements[4] = { u0,u1,u2,u3 };
	float vElements[4] = { v0,v1,v2,v3 };

	for (int i = 0; i < 4; i++)
	{
		verticesData[stride*i + uStart] = uElements[i];
		verticesData[stride*i + vStart] = vElements[i];
	}

	rend->bindSpriteBuffers(vbo, vao, verticesData, vertexAmount);
}

void Sprite::draw()
{
	if (_transparency) {
		blendSprite();
	}
	rend->bindTexture(texture);
	rend->drawShape(TRS, vbo, vao, verticesData, vertexAmount, indexAmount);
	if (_transparency) {
		unblendSprite();
	}
}

void Sprite::blendSprite() {
	rend->blendTexture();
}

void Sprite::unblendSprite() {
	rend->unblendTexture();
}