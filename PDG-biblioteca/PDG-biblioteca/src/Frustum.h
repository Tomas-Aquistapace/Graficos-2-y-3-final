#pragma once
#include "Exports.h"
#include <glm/vec3.hpp>
#include "Plane.h"
#include "Camera.h"
#include <vector>

class Camera;

class AQUISTAPACE_API Frustum
{
public:
	Frustum(Camera* cam, float aspect, float fovY, float zNear, float zFar);
	bool pointInFrustum(glm::vec3 position);
	~Frustum();
	std::vector<Plane*> getPlanes();
private:
	std::vector<Plane*> planes;
};

