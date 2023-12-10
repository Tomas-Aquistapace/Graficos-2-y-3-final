#pragma once
#include <glm/vec3.hpp>
#include "Exports.h"
#include "Renderer.h"

enum LightType
{
	test,
	point,
	directional,
	spot
};

class AQUISTAPACE_API Lightning
{
	Renderer* _rend;

	bool _active;

	static int dirLights;
	static int pointLights;
	static int spotLights;

	glm::vec3 _pos;
	glm::vec3 _dir;

	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;

	float _constant;
	float _linear;
	float _quadratic;

	float _cutOff;

	LightType _lightType;

	void setLight();
public:
	int id;
	Lightning(Renderer* rend);
	void initializePoint(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float linear, float quadratic);
	void initializeDirectional(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void initializeSpot(glm::vec3 pos, glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float linear, float quadratic);
	void setPos(glm::vec3 newPos);
	glm::vec3 getPos();
	void setDir(glm::vec3 newDir);
	glm::vec3 getDir();
	void setActiveState(bool newActiveState);
	bool getActiveState();
};