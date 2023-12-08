#pragma once
#include "Exports.h"
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat2x2.hpp>
class SABASAENGINE_API Plane
{
	// unit vector
	
	// distance from origin to the nearest point in the plane
	float     distance = 0.f;
public:
	glm::vec3 normal;
	Plane();
	Plane(const glm::vec3& p1, const glm::vec3& norm);
	~Plane();

	float getSignedDistanceToPlane(const glm::vec3& point);

	bool IsOnPositiveNormal(const glm::vec3& point);
};

