#pragma once
#include "Renderer.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Exports.h"

using namespace glm;
using namespace std;
class SABASAENGINE_API Entity
{
protected:
	mat4 translateMat;
	mat4 rotateXMat;
	mat4 rotateYMat;
	mat4 rotateZMat;
	mat4 scaleMat;
	Renderer* rend;

	vec3 posVec;
	vec3 rotVec;
	vec3 scaleVec;

public:
	mat4 TRS;
	virtual void UpdateTRS();
	Entity();
	Entity(Renderer* renderer);
	vec3 getPosition();
	virtual void setPosition(vec3 newPosition);
	vec3 getRotation();
	virtual void setRotationX(float x);	//despues crear una funcion para setear 
	virtual void setRotationY(float y);	//todas las rotaciones juntas
	virtual void setRotationZ(float z);
	virtual void setRotation(vec3 newRot);
	vec3 getRight();
	vec3 getUp();
	vec3 getBackward();
	vec3 getForward();
	vec3 getScale();
	virtual void setScale(vec3 newScale);
	void setBaseMatrices(mat4 translate, mat4 rotateX, mat4 rotateY, mat4 rotateZ, mat4 scale);
	mat4 getTRS();
};
