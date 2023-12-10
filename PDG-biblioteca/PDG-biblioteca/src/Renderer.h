#pragma once
#include "Exports.h"
#include "Window.h"
#include "Material.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace glm;

class AQUISTAPACE_API Renderer
{
private:
	GLuint _vertexShader;
	GLuint _fragmentShader;

	unsigned int _shaderProgram;

	int _posAttrib;
	int _textureAttrib;
	int _normalAttrib;
	int posElementsAmount = 3;
	int texElementsAmount = 2;
	int normalElementsAmount = 3;
	int attribElementsAmount = 8;

	Material* defaultMat;
	void setMaterial(Material* material);

	unsigned int compileShader(unsigned int type, const char* source);
	void deleteShaders();

	void setPosAttrib();
	void setNormalAttrib();
	void setTextureAttrib();
	void setSpriteAttrib();

	void setVP();

public:
	Renderer();
	~Renderer();

	void initShaders(const char* vertex, const char* fragment);
	void initShaderProgram();

	void creatoVAO(unsigned int& vao);
	void createVBO(float* vertexDataArray, int arraySize, unsigned int& vbo);
	void createEBO(int* indexArray, int arraySize, unsigned int& _ebo);

	void drawShape(mat4x4 trs, unsigned int vbo, unsigned int vao, float* vertex, unsigned int size, unsigned int indexSize);
	void drawShape(mat4x4 trs, unsigned int vbo, unsigned int vao, float* vertex, unsigned int size, unsigned int indexSize, Material* material);

	unsigned int getShaderProgram();

	void setTexture(unsigned int texture);
	void bindSpriteBuffers(unsigned int vbo, unsigned int vao, float* vertex, float size);
	void bindTexture(unsigned int texture);
	void setMesh(string locationName, int texNumber, bool usesSpecularMaps);

	void updateProgram(mat4 model);
	void blendTexture();
	void unblendTexture();

	void updateView(vec3 position, vec3 target);
	void updateView(vec3 position, vec3 front, vec3 up);
	void updateLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float constant, float linear, float quadratic, float cutOff, unsigned int lightKind, bool activeState, int id);

	int getAttribElementsAmount();
};