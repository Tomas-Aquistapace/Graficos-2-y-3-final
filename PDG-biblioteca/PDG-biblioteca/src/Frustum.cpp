#include "Frustum.h"

Frustum::Frustum(Camera* cam, float aspect, float fovY, float zNear, float zFar)
{
	const float halfVSide = zFar * tanf(fovY * .5f);
	const float halfHSide = halfVSide * aspect;
	const glm::vec3 frontMultFar = zFar * cam->getFront();
	system("CLS");
	
	std::cout << "Near Face ";
	planes.push_back(new Plane(cam->getPosition() + zNear * cam->getFront(), cam->getFront()));
	std::cout << "Far Face ";
	planes.push_back(new Plane(cam->getPosition() + frontMultFar, -cam->getFront()));
	std::cout << "Right Face ";
	planes.push_back(new Plane(cam->getPosition(),
				glm::cross(frontMultFar - cam->getSide() *halfHSide, cam->getUp())));
	std::cout << "Left Face ";
	planes.push_back(new Plane(cam->getPosition(),
				glm::cross(cam->getUp(), frontMultFar + cam->getSide() * halfHSide)));
	std::cout << "Top Face ";
	planes.push_back(new Plane(cam->getPosition(),
				glm::cross(cam->getSide(),frontMultFar - cam->getUp() * halfVSide)));
	std::cout << "Bottom Face ";
	planes.push_back(new Plane(cam->getPosition(),
				glm::cross(frontMultFar + cam->getUp() * halfVSide, cam->getSide())));
}

bool Frustum::pointInFrustum(glm::vec3 point)
{
	for (int i = 0; i < planes.size(); i++) 
	{
		if (!planes[i]->IsOnPositiveNormal(point))
		{
			return false;
		}
	}
	return true;
}

std::vector<Plane*> Frustum::getPlanes() 
{
	return planes;
}

Frustum::~Frustum()
{

}