#include "Entity.h"
#include <glm/ext/matrix_transform.hpp> 


Entity::Entity()
{
	posVec = vec3(0.0f);
	rotVec = vec3(0.0f);
	scaleVec = vec3(1.0f);

	translateMat = mat4(1.0f);
	rotateXMat = mat4(1.0f);
	rotateYMat = mat4(1.0f);
	rotateZMat = mat4(1.0f);
	scaleMat = mat4(1.0f);
	TRS = mat4(1.0f);
}

Entity::Entity(Renderer* renderer)
{
	rend = renderer;

	posVec = vec3(0.0f);
	rotVec = vec3(0.0f);
	scaleVec = vec3(1.0f);

	translateMat = mat4(1.0f);
	rotateXMat = mat4(1.0f);
	rotateYMat = mat4(1.0f);
	rotateZMat = mat4(1.0f);
	scaleMat = mat4(1.0f);
	TRS = mat4(1.0f);
}
vec3 Entity::getRight()
{
	return TRS[0];
}

vec3 Entity::getUp()
{
	return TRS[1];
}

vec3 Entity::getBackward()
{
	return TRS[2];
}

vec3 Entity::getForward()
{
	return -TRS[2];
}
// 1) UpdateTRS parameter at the draw function - Done
/* function()
	{
		TRS = translateMat * rotateXMat * rotateYMat * rotateZMat * scaleMat;
		If (parent)
		{
			TRS = parent.TRS *TRS;
		}
	}
*/
// 2) Should take parent's trs and multiply input - Done
// 3) Calculate the matrices on the fly. - Ask

void Entity::UpdateTRS()
{
	TRS = translateMat * rotateXMat * rotateYMat * rotateZMat * scaleMat;
}

vec3 Entity::getPosition()
{
	return posVec;
}

void Entity::setPosition(vec3 newPosition)
{
	posVec = newPosition;
	translateMat = translate(mat4(1.0f), posVec);
	UpdateTRS();
}

vec3 Entity::getRotation()
{
	return rotVec;
}

void Entity::setRotationX(float newRotX)
{
	rotVec.x = newRotX;
	vec3 rot;
	rot = vec3(1.0f, 0.0f, 0.0f);
	rotateXMat = rotate(mat4(1.0f), glm::radians(newRotX), rot);
	UpdateTRS();
}

void Entity::setRotationY(float newRotY)
{
	rotVec.y = newRotY;
	vec3 rot;
	rot = vec3(0.0f, 1.0f, 0.0f);
	rotateYMat = rotate(mat4(1.0f), glm::radians(newRotY), rot);
	UpdateTRS();
}

void Entity::setRotationZ(float newRotZ)
{
	rotVec.z = newRotZ;
	vec3 rot;
	rot = vec3(0.0f, 0.0f, 1.0f);
	rotateZMat = rotate(mat4(1.0f), glm::radians(newRotZ), rot);
	UpdateTRS();
}

void Entity::setRotation(vec3 newRot)
{
	vec3 rot;
	if (newRot.x != rotVec.x)
	{
		rotVec.x = newRot.x;
		rot = vec3(1.0f, 0.0f, 0.0f);
		rotateXMat = rotate(mat4(1.0f), glm::radians(newRot.x), rot);
		UpdateTRS();
	}

	if (newRot.y != rotVec.y)
	{
		rotVec.y = newRot.y;
		rot = vec3(0.0f, 1.0f, 0.0f);
		rotateYMat = rotate(mat4(1.0f), glm::radians(newRot.y), rot);
		UpdateTRS();
	}

	if (newRot.z != rotVec.z)
	{
		rotVec.z = newRot.z;
		rot = vec3(0.0f, 0.0f, 1.0f);
		rotateZMat = rotate(mat4(1.0f), glm::radians(newRot.z), rot);
		UpdateTRS();
	}
}

vec3 Entity::getScale()
{
	return scaleVec;
}

void Entity::setScale(vec3 newScale)
{
	scaleVec = newScale;
	scaleMat = scale(mat4(1.0f),scaleVec);
	UpdateTRS();
}

void Entity::setBaseMatrices(mat4 translate, mat4 rotateX, mat4 rotateY, mat4 rotateZ, mat4 scale)
{
	translateMat = translate;
	rotateXMat = rotateX;
	rotateYMat = rotateY;
	rotateZMat = rotateZ;
	scaleMat = scale;
	UpdateTRS();
}

mat4 Entity::getTRS()
{
	return TRS;
}

