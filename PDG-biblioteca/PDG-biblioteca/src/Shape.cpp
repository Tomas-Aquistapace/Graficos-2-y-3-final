#include "Shape.h"
#include "glm/gtc/type_ptr.hpp"

Shape::Shape(ShapeTypes shapeType, Renderer* renderer):Entity(renderer)
{
	rend = renderer;
	_shapeType = shapeType;
	vertexToUse = NULL;
	switch (_shapeType)
	{
	case triangle:
		createTriangle();
		break;
	case cube:
		createCube();
		break;
	default:
	case rectangle:
		createRectangle();
		break;
	}
	mat = new Material();
	mat->_ambient =  glm::vec3(1.0f, 0.5f, 0.31f);
	mat->_diffuse =  glm::vec3(1.0f, 0.5f, 0.31f);
	mat->_specular = glm::vec3(0.5f, 0.5f, 0.5f);
	mat->_shininess = 32.0f;
}

Shape::Shape(ShapeTypes shapeType, Material newMat, Renderer* renderer) :Entity(renderer)
{
	rend = renderer;
	_shapeType = shapeType;
	vertexToUse = NULL;
	switch (_shapeType)
	{
	case triangle:
		createTriangle();
		break;
	case cube:
		createCube();
		break;
	default:
	case rectangle:
		createRectangle();
		break;
	}
	mat = new Material(newMat._ambient,newMat._diffuse,newMat._specular,newMat._shininess);

}

Shape::~Shape()
{
	if (mat)delete mat;
}

void Shape::createCube()
{
	vertexAmount = 288;
	indexAmount = 36;
	width = 0.25f - (-0.25f);
	height = 0.5f - (-0.5f);
	int indicesData[36] =
	{
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,
		18,19,16,

		20,21,22,
		22,23,20
	};
	vertexToUse = cubeVerticesData;
	rend->creatoVAO(vao);
	rend->createVBO(vertexToUse, vertexAmount, vbo);
	rend->createEBO(indicesData, indexAmount, ebo);
	Texture* texture = new Texture("res/white.png");
	texture->Bind(0);
	defaultTexture = texture->getTex();
	rend->setTexture(defaultTexture);
}

void Shape::createRectangle()
{
	vertexAmount = 32;
	indexAmount = 6;
	width = 0.25f - (-0.25f);
	height = 0.5f - (-0.5f);
	int indicesData[6] =
	{
	    0, 1, 3,
		1, 2, 3
	};
	vertexToUse = recVerticesData;
	rend->creatoVAO(vao);
	rend->createVBO(vertexToUse, vertexAmount, vbo);
	rend->createEBO(indicesData, indexAmount, ebo);
	Texture* texture = new Texture("res/white.png");
	texture->Bind(0);
	defaultTexture = texture->getTex();
	rend->setTexture(defaultTexture);
}

void Shape::createTriangle()
{
	vertexAmount = 24;
	indexAmount = 3;
	int indicesData[3] =
	{
		0, 1, 2
	};
	width = 0.25f - (-0.25f);
	height = 0.5f - (-0.5f);
	vertexToUse = triVerticesData;
	rend->creatoVAO(vao);
	rend->createVBO(vertexToUse, vertexAmount, vbo);
	rend->createEBO(indicesData,indexAmount,ebo);
	Texture* texture = new Texture("res/white.png");
	texture->Bind(0);
	defaultTexture = texture->getTex();
	rend->setTexture(defaultTexture);
}

void Shape::draw()
{
	glBindTexture(GL_TEXTURE_2D, defaultTexture);
	glActiveTexture(GL_TEXTURE0);

	rend->drawShape(TRS, vbo, vao, vertexToUse, vertexAmount, indexAmount, mat);
}