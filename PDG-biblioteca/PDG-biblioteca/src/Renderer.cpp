#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

#pragma region PRIVATE_METHODS
unsigned int Renderer::compileShader(unsigned int type, const char* source)
{
	unsigned int id = glCreateShader(type);

	string sourceShaderCode;
	ifstream sourceShaderFile;

	sourceShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		sourceShaderFile.open(source);
		std::stringstream sourceShaderStream;

		sourceShaderStream << sourceShaderFile.rdbuf();

		sourceShaderFile.close();

		sourceShaderCode = sourceShaderStream.str();
	}
	catch (std::ifstream::failure& e) { std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl; }

	const char* sourceCode = sourceShaderCode.c_str();

	glShaderSource(id, 1, &sourceCode, nullptr);
	glCompileShader(id);

	return id;
}

void Renderer::deleteShaders()
{
	glDeleteProgram(_shaderProgram);
	glDeleteShader(_fragmentShader);
	glDeleteShader(_vertexShader);
}

void Renderer::setMaterial(Material* material)
{
	unsigned int uniformMatAmb = glGetUniformLocation(_shaderProgram, "mat.ambient");
	unsigned int uniformMatDiff = glGetUniformLocation(_shaderProgram, "mat.diffuse");
	unsigned int uniformMatSpec = glGetUniformLocation(_shaderProgram, "mat.specular");
	unsigned int uniformMatShin = glGetUniformLocation(_shaderProgram, "mat.shininess");
	glUniform3fv(uniformMatAmb, 1, value_ptr(material->_ambient));
	glUniform3fv(uniformMatDiff, 1, value_ptr(material->_diffuse));
	glUniform3fv(uniformMatSpec, 1, value_ptr(material->_specular));
	glUniform1f(uniformMatShin, material->_shininess);
}

void Renderer::setPosAttrib()
{
	_posAttrib = glGetAttribLocation(_shaderProgram, "position");
	glVertexAttribPointer(_posAttrib, posElementsAmount, GL_FLOAT, GL_FALSE, attribElementsAmount * sizeof(float), 0);
	glEnableVertexAttribArray(_posAttrib); //cambie esto de 0 a _posAttrib
}

void Renderer::setTextureAttrib()
{
	_textureAttrib = glGetAttribLocation(_shaderProgram, "texCoor");
	glVertexAttribPointer(_textureAttrib, texElementsAmount, GL_FLOAT, GL_FALSE, attribElementsAmount * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(_textureAttrib);//cambie esto de 1 a _textureAttrib
}

void Renderer::setNormalAttrib()
{
	_normalAttrib = glGetAttribLocation(_shaderProgram, "normal");
	glVertexAttribPointer(_normalAttrib, normalElementsAmount, GL_FLOAT, GL_FALSE, attribElementsAmount * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(_normalAttrib);//cambie esto de 1 a _textureAttrib
}

void Renderer::setSpriteAttrib()
{
	glUniform1i((glGetUniformLocation(_shaderProgram, "tex")), 0);// CHECKEAR ESTO
	setPosAttrib();
	setTextureAttrib();
	setNormalAttrib();
}

void Renderer::setVP()
{
	unsigned int projectionLocation = glGetUniformLocation(_shaderProgram, "Projection");
	unsigned int viewLocation = glGetUniformLocation(_shaderProgram, "View");
	mat4 proj = mat4(1.0f);
	mat4 view = mat4(1.0f);
	view = lookAt(vec3(0.0, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	/*Ortografica: glm::ortho(-4.0f,4.0f,-2.0f,2.0f,-100.0f,100.0f); */
	proj = perspective(45.0f, 2.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(proj));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
}
#pragma endregion

#pragma region CONSTRUCTOR_METHODS
Renderer::Renderer()
{
	defaultMat = new Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.5f, 0.5f, 0.5f), 32.0f);
}

Renderer::~Renderer()
{
	deleteShaders();

	if (defaultMat) delete defaultMat;
}
#pragma endregion

#pragma region PUBLIC_METHODS
void Renderer::initShaders(const char* vertexPath, const char* fragmentPath)
{
	_vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
	_fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);
}

void Renderer::initShaderProgram()
{
	_shaderProgram = glCreateProgram();

	glAttachShader(_shaderProgram, _vertexShader);
	glAttachShader(_shaderProgram, _fragmentShader);
	glLinkProgram(_shaderProgram);
	glValidateProgram(_shaderProgram);
	glUseProgram(_shaderProgram);

	setVP();
	// Crear Frustum con la data de la camara.
}

void Renderer::creatoVAO(unsigned int& vao)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void Renderer::createVBO(float* vertexDataArray, int arraySize, unsigned int& vbo)
{
	int vertexSize = sizeof(vertexDataArray) * arraySize;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexDataArray, GL_STATIC_DRAW);
}

void Renderer::createEBO(int* indexArray, int arraySize, unsigned int& _ebo)
{
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray) * arraySize, indexArray, GL_STATIC_DRAW);
}

void Renderer::drawShape(glm::mat4x4 trs, unsigned int vbo, unsigned int vao, float* vertex, unsigned int size, unsigned int indexSize)
{
	setMaterial(defaultMat);
	bindSpriteBuffers(vbo, vao, vertex, size);
	setSpriteAttrib();
	updateProgram(trs);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::drawShape(glm::mat4x4 trs, unsigned int vbo, unsigned int vao, float* vertex, unsigned int size, unsigned int indexSize, Material* material)
{
	setMaterial(material);
	bindSpriteBuffers(vbo, vao, vertex, size);
	setSpriteAttrib();
	glUniform1i(glGetUniformLocation(_shaderProgram, "isModel"), 0);
	updateProgram(trs);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
}

unsigned int Renderer::getShaderProgram()
{
	return _shaderProgram;
}

void Renderer::setTexture(unsigned int texture)
{
	//cout << texture << endl;
	unsigned int uniformTex = glGetUniformLocation(_shaderProgram, "tex");
	glUseProgram(_shaderProgram);
	glUniform1i(uniformTex, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
}

void Renderer::bindSpriteBuffers(unsigned int vbo, unsigned int vao, float* vertex, float size)
{
	unsigned int memorySize = sizeof(float) * size;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, memorySize, vertex, GL_STATIC_DRAW);
}

void Renderer::bindTexture(unsigned int texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
}

void Renderer::updateProgram(mat4 model)
{
	unsigned int transformLocation = glGetUniformLocation(_shaderProgram, "Model");
	glUseProgram(_shaderProgram);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, value_ptr(model));
}

void Renderer::blendTexture()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::unblendTexture()
{
	glDisable(GL_BLEND);
}

void Renderer::updateView(vec3 position, vec3 target)
{
	unsigned int viewLocation = glGetUniformLocation(_shaderProgram, "View");
	mat4 view = lookAt(position, target, vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));

	unsigned int viewPos = glGetUniformLocation(_shaderProgram, "viewPos");
	glUniform3fv(viewPos, 1, value_ptr(position));
}

void Renderer::updateView(vec3 position, vec3 front, vec3 up)
{
	unsigned int viewLocation = glGetUniformLocation(_shaderProgram, "View");
	mat4 view;
	view = lookAt(position, position + front, up);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));

	unsigned int viewPos = glGetUniformLocation(_shaderProgram, "viewPos");
	glUniform3fv(viewPos, 1, value_ptr(position));
}

int Renderer::getAttribElementsAmount()
{
	return attribElementsAmount;
}

void Renderer::updateLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, unsigned int lightKind, bool activeState, int id)
{
	string dirLightStr = "dirLight";
	string pointLightStr = "pointLight";
	string spotLightStr = "spotLight";
	string testLightStr = "light";

	string indexStr = "[" + to_string(id) + "]";
	pointLightStr += indexStr;
	dirLightStr += indexStr;
	spotLightStr += indexStr;

	switch (lightKind)
	{
	default:
	case 0:
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr + ".position").c_str()), 1, value_ptr(position));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr + ".ambient").c_str()), 1, value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr + ".diffuse").c_str()), 1, value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr + ".specular").c_str()), 1, value_ptr(specular));
		glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr + ".initialized").c_str()), activeState);
		glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr + ".id").c_str()), id);
		break;
	case 1:
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".position").c_str()), 1, value_ptr(position));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".ambient").c_str()), 1, value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".diffuse").c_str()), 1, value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".specular").c_str()), 1, value_ptr(specular));
		glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".constant").c_str()), constant);// 1.0f
		glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".linear").c_str()), linear);// 0.09f
		glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".quadratic").c_str()), quadratic);// 0.032f
		glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".initialized").c_str()), activeState);
		glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr + ".id").c_str()), id);
		break;
	case 2:
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr + ".direction").c_str()), 1, value_ptr(direction));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr + ".ambient").c_str()), 1, value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr + ".diffuse").c_str()), 1, value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr + ".specular").c_str()), 1, value_ptr(specular));
		glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr + ".initialized").c_str()), activeState);
		glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr + ".id").c_str()), id);
		break;
	case 3:
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".position").c_str()), 1, value_ptr(position));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".direction").c_str()), 1, value_ptr(direction));//vec3(0.0f, 1.0f, 0.0f)
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".ambient").c_str()), 1, value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".diffuse").c_str()), 1, value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".specular").c_str()), 1, value_ptr(specular));
		glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".cutOff").c_str()), glm::cos(cutOff));// glm::radians(12.5f)
		glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".constant").c_str()), constant);// 1.0f
		glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".linear").c_str()), linear);// 0.09f
		glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".quadratic").c_str()), quadratic);// 0.032f
		glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".initialized").c_str()), activeState);
		glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".id").c_str()), id);
		break;
	}
}

void Renderer::setMesh(std::string locationName, int texNumber, bool usesSpecularMaps)
{
	glUseProgram(_shaderProgram);
	glUniform1i(glGetUniformLocation(_shaderProgram, (GLchar*)locationName.c_str()), texNumber);
	setMaterial(defaultMat);
	glUniform1i(glGetUniformLocation(_shaderProgram, "mat.useSpecularMaps"), usesSpecularMaps);

	if (!usesSpecularMaps)
	{
		glUniform3fv(glGetUniformLocation(_shaderProgram, "mat.specular"), 1, value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));

	}
	glUniform1i(glGetUniformLocation(_shaderProgram, "isModel"), true);
}
#pragma endregion