#pragma once
#include "Entity.h"
#include "glm/mat4x4.hpp"
#include "Exports.h"
#include "Texture.h"
#include "Animation.h"
#include "Timer.h"

using namespace glm;


class AQUISTAPACE_API Sprite : public Entity
{
	Texture* textureUsed;
	const int vertexAmount = 32;
	const int indexAmount = 6;
	int indicesData[6] =
	{
		0, 1, 3,
		1, 2, 3
	};
	float baseWidth =  1.0f;
	float baseHeight = 1.0f;
	int _currentFrame;
	int _prevFrame;
	Animation* _animation;
	void initSprite(const char* path);
	void initSprite(unsigned int newTexture);
	void initSprite(unsigned int newTexture, float newWidth, float newHeight);
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
protected:	
	float verticesData[32] = {
		 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f,  -1.0f,
		 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f,
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  -1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  -1.0f
	};
	float _scaleX;
	float _scaleY;
	bool _transparency;
	unsigned int texture;
public:
	Sprite(Renderer* renderer, const char* path,bool transparency);
	Sprite(Renderer* renderer, unsigned int newTexture,bool transparency);
	Sprite(Renderer* renderer, unsigned int newTexture, bool transparency, float newWidth, float newHeight);
	~Sprite();
	void draw();
	void updateSprite(Timer& timer);
	void setAnimation(Animation* animation) { _animation = animation; }
	void setTextureCoordinates(float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3);
	float width;
	float height;
	void blendSprite();
	void unblendSprite();
};