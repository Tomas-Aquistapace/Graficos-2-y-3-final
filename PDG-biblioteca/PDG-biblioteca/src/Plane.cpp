#include "Plane.h"


Plane::Plane() {
	normal = { 0.f, 1.f, 0.f };
}

Plane::Plane(const glm::vec3& position, const glm::vec3& norm)
{
	normal = glm::normalize(norm);
	distance = glm::dot(normal, position);
	std::cout << "Normal: " << normal.x << " " << normal.y << " " << normal.z << std::endl;
};

Plane::~Plane() {

}

float Plane::getSignedDistanceToPlane(const glm::vec3& point)
{
	return glm::dot(normal, point) - distance;
}

bool Plane::IsOnPositiveNormal(const glm::vec3& point)
{
	if (getSignedDistanceToPlane(point) >= 0) {
		return true;
	}
	else
	{
		return false;
	}
}