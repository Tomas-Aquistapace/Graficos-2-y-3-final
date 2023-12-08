#pragma once
#include "Exports.h"
#include "Window.h"
#include "Material.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

class SABASAENGINE_API Renderer
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
	public:
		Renderer();
		~Renderer();
		void initVertexShader();
		void initFragmentShader();
		void initShaderProgram();

		void creatoVAO(unsigned int &vao);
		void createVBO(float* vertexDataArray, int arraySize, unsigned int &vbo);
		void createEBO(int* indexArray, int arraySize, unsigned int &_ebo);
		
		void setPosAttrib();
		void setTextureAttrib();
		void setNormalAttrib();
		
		void deleteShaderProgram();
		void deleteFragmentShader();
		void deleteVertexShader();
		
		void drawShape(glm::mat4x4 trs, unsigned int vbo, unsigned int vao, float* vertex, unsigned int size, unsigned int indexSize);
		void drawShape(glm::mat4x4 trs, unsigned int vbo, unsigned int vao, float* vertex, unsigned int size, unsigned int indexSize, Material* material);
		
		unsigned int getShaderProgram();
		
		void setTexture(unsigned int texture);
		void bindSpriteBuffers(unsigned int vbo, unsigned int vao, float* vertex, float size);
		void setSpriteAttrib();
		void bindTexture(unsigned int texture);
		void setMesh(std::string locationName, int texNumber, bool usesSpecularMaps);
		
		void updateProgram(glm::mat4 model);
		void blendTexture();
		void unblendTexture();
		void setVP();
		void updateView(glm::vec3 position, glm::vec3 target);
		void updateView(glm::vec3 position, glm::vec3 front, glm::vec3 up);
		void updateLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, unsigned int lightKind, bool activeState, int id);
		
		int getAttribElementsAmount();
};