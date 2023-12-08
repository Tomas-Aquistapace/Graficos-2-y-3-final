#pragma once
#include "Entity.h"
#include "glm/mat4x4.hpp"
#include "Exports.h"
#include "Texture.h"
#include "Material.h"

using namespace glm;

enum ShapeTypes
{
	rectangle,
	triangle,
	cube
};

class SABASAENGINE_API Shape : public Entity
{
private:
	void createRectangle();
	void createTriangle();
	void createCube();
	unsigned int vertexAmount = 32;
	unsigned int indexAmount = 6;
	float recVerticesData[32] =
	{
		 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f,  -1.0f,
		 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f,
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  -1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  -1.0f
	};
	float triVerticesData[24] =
	{
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f,   -1.0f,
		 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f,  0.0f,   -1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  -1.0f
	};
	float cubeVerticesData[288] =
	{
		-0.5f, -0.5f, -0.5f,    1.0f, 0.0f,      0.0f,  0.0f, -1.0f, //0
		 0.5f, -0.5f, -0.5f,    0.0f, 0.0f,      0.0f,  0.0f, -1.0f,//1
		 0.5f,  0.5f, -0.5f,    0.0f, 1.0f,      0.0f,  0.0f, -1.0f,//2
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f,      0.0f,  0.0f, -1.0f,//3

		-0.5f, -0.5f,  0.5f,    1.0f, 0.0f,      0.0f,  0.0f, 1.0f, //4
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,      0.0f,  0.0f, 1.0f, //5
		 0.5f,  0.5f,  0.5f,    0.0f, 1.0f,      0.0f,  0.0f, 1.0f, //6
		-0.5f,  0.5f,  0.5f,    1.0f, 1.0f,      0.0f,  0.0f, 1.0f,//7
															  
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     -1.0f,  0.0f,  0.0f, //8
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     -1.0f,  0.0f,  0.0f, //9
		-0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     -1.0f,  0.0f,  0.0f, //10
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,		-1.0f,  0.0f,  0.0f, //11

		 0.5f,  0.5f,  0.5f,    0.0f, 1.0f,      1.0f,  0.0f,  0.0f, //12
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,      1.0f,  0.0f,  0.0f, //13
		 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,      1.0f,  0.0f,  0.0f, //14
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,      1.0f,  0.0f,  0.0f, //15

		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,      0.0f,  -1.0f,  0.0f, //16
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,      0.0f,  -1.0f,  0.0f, //17
		 0.5f, -0.5f,  0.5f,    1.0f, 1.0f,      0.0f,  -1.0f,  0.0f, //18
		-0.5f, -0.5f,  0.5f,    1.0f, 0.0f,      0.0f,  -1.0f,  0.0f, //19

		-0.5f,  0.5f, -0.5f,    0.0f, 0.0f,      0.0f,  1.0f,  0.0f, //20
		 0.5f,  0.5f, -0.5f,    0.0f, 1.0f,      0.0f,  1.0f,  0.0f, //21
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,      0.0f,  1.0f,  0.0f, //22
		-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,      0.0f,  1.0f,  0.0f, //23
	};
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int defaultTexture;
	ShapeTypes _shapeType;
	float* vertexToUse;
	Material* mat;
public:
	Shape(ShapeTypes shapeType, Renderer* renderer);
	Shape(ShapeTypes shapeType, Material newMat, Renderer* renderer);
	~Shape();
	void draw();
	float width;
	float height;
};

