#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace glm;

const GLchar* vertexSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoor;
layout (location = 2) in vec3 normal;

out vec2 texCoord;
out vec3 fposition;
out vec3 fnormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;	

void main()
{
gl_Position = Projection * View * Model * vec4(position, 1.0);
fnormal = mat3(transpose(inverse(Model))) * normal;
fposition = vec4(Model*vec4(position,1.0f)).xyz;
texCoord = texCoor;
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 330 core

struct Material {
    vec3 diffuse;
    vec3 specular;    
    float shininess;
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_diffuse4;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_specular4;
	sampler2D texture_normal1;
	sampler2D texture_height1;

	bool useSpecularMaps;

}; 

struct MaterialResults {
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

	float cutOff;
	int initialized;
};

out vec4 outColor;

in vec2 texCoord;
in vec3 fposition;
in vec3 fnormal;

uniform sampler2D tex;
uniform sampler2D texture_diffuse1;

#define MAX_DIR_LIGHTS 6
#define MAX_POINT_LIGHTS 6
#define MAX_SPOT_LIGHTS 6

uniform Light light;
uniform Light dirLight[MAX_DIR_LIGHTS];
uniform Light pointLight[MAX_POINT_LIGHTS];
uniform Light spotLight[MAX_SPOT_LIGHTS];

uniform int id;

uniform Material mat;
uniform vec3 viewPos;

MaterialResults finalMat;
vec3 calculateLight();
vec3 calculatePointLight(int index);
vec3 calculateDirLight(int index);
vec3 calculateSpotLight(int index);
vec3 unifyDiffuse();
vec3 unifySpecular();

uniform bool isModel;

void main()
{	
vec4 texColor = vec4(0.0f,0.0f,0.0f,0.0f);

if(isModel)
{
	texColor = texture(texture_diffuse1, texCoord);
	finalMat.diffuse = unifyDiffuse();
	finalMat.ambient = unifyDiffuse();
	if (mat.useSpecularMaps)
		finalMat.specular = unifySpecular();
	else
		finalMat.specular = mat.specular;
	finalMat.shininess = 32.0f;
}
else
{
	texColor = texture(tex, texCoord);
	finalMat.diffuse = mat.diffuse;
	finalMat.specular = mat.specular;
	finalMat.ambient = mat.diffuse;
	finalMat.shininess = mat.shininess;
}

vec3 lightResultTest = calculateLight();

vec3 lightResultPoint = vec3(0.0f,0.0f,0.0f);
for (int i = 0; i < MAX_POINT_LIGHTS; i++)
{
	lightResultPoint += calculatePointLight(i);
}

vec3 lightResultDir = vec3(0.0f,0.0f,0.0f);
for (int i = 0; i < MAX_DIR_LIGHTS; i++)
{
	lightResultDir += calculateDirLight(i);
}

vec3 lightResultSpot = vec3(0.0f,0.0f,0.0f);
for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
{
	lightResultSpot += calculateSpotLight(i);
}
vec3 lightResultTotal = /*lightResultTest +*/ lightResultPoint + lightResultDir + lightResultSpot;

vec3 baseAmbient = finalMat.ambient * vec3(0.04f,0.04f,0.04f);

outColor = vec4(lightResultTotal, 1.0f);
if (isModel) outColor = vec4(lightResultTotal, 1.0f);

outColor+= vec4(baseAmbient,1.0f); 
}

vec3 calculateLight()
{
	if (light.initialized != 1) return vec3(0.0f,0.0f,0.0f);

	vec3 ambientLight = vec3(0.0f);
	vec3 specular= vec3(0.0f);
	vec3 diffuseFinal= vec3(0.0f);
		
	vec3 posToLightDirVec = normalize(light.position - fposition);

	//Ambient Light
	ambientLight = light.ambient * finalMat.ambient;
	
	//Diffuse Light
	float diffuse = clamp(dot(posToLightDirVec,fnormal),0,1); // producto punto entre la distancia con la luz y la normal
	diffuseFinal = light.diffuse * (diffuse* finalMat.diffuse);
	
	// Specular Light
	vec3 viewDir = normalize(viewPos-fposition);
	vec3 reflectDir = reflect(-posToLightDirVec,fnormal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0f), finalMat.shininess);	
	specular = light.specular * (spec * finalMat.specular);
	
	vec3 lightResult = ambientLight + diffuseFinal + specular;
	
	return lightResult;
}

vec3 calculateDirLight(int index)
{
	if (dirLight[index].initialized != 1) return vec3(0.0f,0.0f,0.0f);
	
	vec3 ambientLight = vec3(0.0f);
	vec3 specular= vec3(0.0f);
	vec3 diffuseFinal= vec3(0.0f);

	vec3 posToLightDirVec = normalize(-dirLight[index].direction);
		
	//Ambient Light
	ambientLight = dirLight[index].ambient * finalMat.ambient;
	
	//Diffuse Light
	float diffuse = clamp(dot(posToLightDirVec,fnormal),0,1); // producto punto entre la distancia con la luz y la normal
	diffuseFinal = dirLight[index].diffuse * (diffuse* finalMat.diffuse);
	
	// Specular Light
	vec3 viewDir = normalize(viewPos-fposition);
	vec3 reflectDir = reflect(-posToLightDirVec,fnormal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0), finalMat.shininess);	
	specular = dirLight[index].specular * (spec * finalMat.specular);
	
	vec3 lightResult = ambientLight + diffuseFinal + specular;

	return lightResult;
}

vec3 calculatePointLight(int index)
{
	if (pointLight[index].initialized != 1) return vec3(0.0f,0.0f,0.0f);
	
	vec3 ambientLight = vec3(0.0f);
	vec3 specular= vec3(0.0f);
	vec3 diffuseFinal= vec3(0.0f);
	
	vec3 posToLightDirVec = normalize(pointLight[index].position - fposition);

	//Ambient Light
	ambientLight = pointLight[index].ambient * finalMat.ambient;
	
	//Diffuse Light
	float diffuse = clamp(dot(posToLightDirVec,fnormal),0,1); // producto punto entre la distancia con la luz y la normal
	diffuseFinal = pointLight[index].diffuse * (diffuse* finalMat.diffuse);
	
	//Specular Light	
	vec3 viewDir = normalize(viewPos-fposition);
	vec3 reflectDir = reflect(-posToLightDirVec,fnormal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0), finalMat.shininess);	
	specular = pointLight[index].specular * (spec * finalMat.specular);
	
	//Atenuation
	float distance    = length(pointLight[index].position - fposition);
	float attenuation = 1.0 / (pointLight[index].constant + pointLight[index].linear * distance + pointLight[index].quadratic * (distance * distance));  

	ambientLight *= attenuation;
	diffuseFinal *= attenuation;
	specular *= attenuation;

	vec3 lightResult = ambientLight + diffuseFinal + specular;

	return lightResult;

}

vec3 calculateSpotLight(int index)
{
	if (spotLight[index].initialized != 1) return vec3(0.0f,0.0f,0.0f);
	
	vec3 ambientLight = vec3(0.0f);
	vec3 specular= vec3(0.0f);
	vec3 diffuseFinal= vec3(0.0f);
		
	//Ambient Light
	
	vec3 posToLightDirVec = normalize(spotLight[index].position - fposition);
	
	float theta = dot(posToLightDirVec, normalize(-spotLight[index].direction));
	
	if(theta > spotLight[index].cutOff) 
	{       
		ambientLight = spotLight[index].ambient * finalMat.ambient;
		//Diffuse Light
		float diffuse = max(dot(fnormal, posToLightDirVec),0.0f); // producto punto entre la distancia con la luz y la normal
		diffuseFinal = spotLight[index].diffuse * (diffuse* finalMat.diffuse);
		
		// Specular Light	
		vec3 viewDir = normalize(viewPos - fposition);
		vec3 reflectDir = reflect(-posToLightDirVec,fnormal);
		float spec = pow(max(dot(viewDir,reflectDir),0.0f), finalMat.shininess);	
		specular = spotLight[index].specular * (spec * finalMat.specular);

		float distance    = length(spotLight[index].position - fposition);
		float attenuation = 1.0 / (spotLight[index].constant + spotLight[index].linear * distance + spotLight[index].quadratic * (distance * distance));  
		
		diffuseFinal *= attenuation;
		specular *= attenuation;

	}	
	vec3 lightResult = ambientLight + diffuseFinal + specular;
	return lightResult;
		
}

vec3 unifyDiffuse()
{
	vec3 finalTex;
	finalTex = texture(mat.texture_diffuse1, texCoord).rgb;
	finalTex += texture(mat.texture_diffuse2, texCoord).rgb;
	finalTex += texture(mat.texture_diffuse3, texCoord).rgb;
	finalTex += texture(mat.texture_diffuse4, texCoord).rgb;
	return finalTex;
}

vec3 unifySpecular()
{
	vec3 finalTex;
	finalTex =  texture(mat.texture_specular1, texCoord).rgb;
	finalTex += texture(mat.texture_specular2, texCoord).rgb;
	finalTex += texture(mat.texture_specular3, texCoord).rgb;
	finalTex += texture(mat.texture_specular4, texCoord).rgb;
	return finalTex;
}

)glsl";

Renderer::Renderer()
{
	defaultMat = new Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.5f, 0.5f, 0.5f), 32.0f);
}
Renderer::~Renderer()
{
	if (defaultMat) delete defaultMat;
}

void Renderer::initVertexShader() {
	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexShader, 1, &vertexSource, NULL);
	glCompileShader(_vertexShader);

}
void Renderer::initFragmentShader() {
	_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(_fragmentShader);
}

void Renderer::initShaderProgram() {
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _vertexShader);
	glAttachShader(_shaderProgram, _fragmentShader);
	glLinkProgram(_shaderProgram);
	glValidateProgram(_shaderProgram);
	glUseProgram(_shaderProgram);
	setVP();
	// Crear Frustum con la data de la camara.
}

void Renderer::creatoVAO(unsigned int &vao)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void Renderer::createVBO(float* vertexDataArray, int arraySize, unsigned int &vbo)
{
	int vertexSize = sizeof(vertexDataArray) * arraySize;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexDataArray, GL_STATIC_DRAW);
}

void Renderer::createEBO(int* indexArray, int arraySize, unsigned int &_ebo) 
{
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray) * arraySize, indexArray, GL_STATIC_DRAW);
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
	glVertexAttribPointer(_textureAttrib, texElementsAmount, GL_FLOAT, GL_FALSE, attribElementsAmount * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(_textureAttrib);//cambie esto de 1 a _textureAttrib
}

void Renderer::setNormalAttrib()
{
	_normalAttrib = glGetAttribLocation(_shaderProgram, "normal");
	glVertexAttribPointer(_normalAttrib, normalElementsAmount, GL_FLOAT, GL_FALSE, attribElementsAmount * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(_normalAttrib);//cambie esto de 1 a _textureAttrib
}

void Renderer::deleteShaderProgram() {
	glDeleteProgram(_shaderProgram);
}

void Renderer::deleteFragmentShader() {
	glDeleteShader(_fragmentShader);
}

void Renderer::deleteVertexShader() {
	glDeleteShader(_vertexShader);
}

void Renderer::drawShape(glm::mat4x4 trs, unsigned int vbo, unsigned int vao, float* vertex, unsigned int size, unsigned int indexSize)
{
	setMaterial(defaultMat);
	bindSpriteBuffers(vbo,vao,vertex,size);
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

void Renderer::bindSpriteBuffers(unsigned int vbo, unsigned int vao, float* vertex, float size) {
	unsigned int memorySize = sizeof(float) * size;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, memorySize, vertex, GL_STATIC_DRAW);
}

void Renderer::setSpriteAttrib() {
	glUniform1i((glGetUniformLocation(_shaderProgram, "tex")), 0);// CHECKEAR ESTO
	setPosAttrib();
	setTextureAttrib();
	setNormalAttrib();
}

void Renderer::bindTexture(unsigned int texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
}

void Renderer::updateProgram(mat4 model) {
	unsigned int transformLocation = glGetUniformLocation( _shaderProgram, "Model");
	glUseProgram(_shaderProgram);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, value_ptr(model));
}

void Renderer::blendTexture() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::unblendTexture() {
	glDisable(GL_BLEND);
}

void Renderer::setVP(){
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

void Renderer::updateView(vec3 position, vec3 target){
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
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr+".position").c_str()), 1, value_ptr(position));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr+".ambient").c_str()), 1, value_ptr(ambient));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr+".diffuse").c_str()), 1, value_ptr(diffuse));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(testLightStr+".specular").c_str()), 1, value_ptr(specular));
			glUniform1i(glGetUniformLocation(_shaderProgram,  (GLchar*)(testLightStr+".initialized").c_str()), activeState);
			glUniform1i(glGetUniformLocation(_shaderProgram,  (GLchar*)(testLightStr+".id").c_str()), id);
			break;
		case 1:
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr+".position").c_str()), 1, value_ptr(position));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr+".ambient").c_str()), 1, value_ptr(ambient));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr+".diffuse").c_str()), 1, value_ptr(diffuse));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(pointLightStr+".specular").c_str()), 1, value_ptr(specular));
			glUniform1f(glGetUniformLocation(_shaderProgram,  (GLchar*)(pointLightStr+".constant").c_str()), constant);// 1.0f
			glUniform1f(glGetUniformLocation(_shaderProgram,  (GLchar*)(pointLightStr+".linear").c_str()), linear);// 0.09f
			glUniform1f(glGetUniformLocation(_shaderProgram,  (GLchar*)(pointLightStr+".quadratic").c_str()), quadratic);// 0.032f
			glUniform1i(glGetUniformLocation(_shaderProgram,  (GLchar*)(pointLightStr+".initialized").c_str()), activeState);
			glUniform1i(glGetUniformLocation(_shaderProgram,  (GLchar*)(pointLightStr+".id").c_str()), id);
			break;
		case 2:
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr+".direction").c_str()), 1, value_ptr(direction));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr+".ambient").c_str()), 1, value_ptr(ambient));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr+".diffuse").c_str()), 1, value_ptr(diffuse));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(dirLightStr+".specular").c_str()), 1, value_ptr(specular));
			glUniform1i(glGetUniformLocation(_shaderProgram,  (GLchar*)(dirLightStr+".initialized").c_str()), activeState);
			glUniform1i(glGetUniformLocation(_shaderProgram,  (GLchar*)(dirLightStr+".id").c_str()), id);
			break;
		case 3:
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".position").c_str()), 1, value_ptr(position));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr+".direction").c_str()), 1, value_ptr(direction));//vec3(0.0f, 1.0f, 0.0f)
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr+".ambient").c_str()), 1, value_ptr(ambient));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr+".diffuse").c_str()), 1, value_ptr(diffuse));
			glUniform3fv(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr+".specular").c_str()), 1, value_ptr(specular));
			glUniform1f(glGetUniformLocation(_shaderProgram,  (GLchar*)(spotLightStr+".cutOff").c_str()), glm::cos(cutOff));// glm::radians(12.5f)
			glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".constant").c_str()), constant);// 1.0f
			glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".linear").c_str()), linear);// 0.09f
			glUniform1f(glGetUniformLocation(_shaderProgram, (GLchar*)(spotLightStr + ".quadratic").c_str()), quadratic);// 0.032f
			glUniform1i(glGetUniformLocation(_shaderProgram,  (GLchar*)(spotLightStr+".initialized").c_str()), activeState);
			glUniform1i(glGetUniformLocation(_shaderProgram,  (GLchar*)(spotLightStr+".id").c_str()), id);
			break;
	}
}

void Renderer::setMesh(std::string locationName, int texNumber, bool usesSpecularMaps)
{
	glUseProgram(_shaderProgram);
	glUniform1i(glGetUniformLocation(_shaderProgram,(GLchar*)locationName.c_str()), texNumber);
	setMaterial(defaultMat);
	glUniform1i(glGetUniformLocation(_shaderProgram, "mat.useSpecularMaps"), usesSpecularMaps);

	if (!usesSpecularMaps)
	{
		glUniform3fv(glGetUniformLocation(_shaderProgram, "mat.specular"), 1, value_ptr(glm::vec3(0.5f,0.5f,0.5f)));
	
	}
	glUniform1i(glGetUniformLocation(_shaderProgram, "isModel"), true);
}