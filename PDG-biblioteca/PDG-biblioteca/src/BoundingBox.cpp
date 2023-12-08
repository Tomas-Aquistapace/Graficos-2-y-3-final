#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max)
{
	center = (max + min) * 0.5f;
	extents = vec3(max.x - center.x, max.y - center.y, max.z - center.z);
}

BoundingBox::BoundingBox(const vec3 inCenter, float inExtentX, float inExtentY, float inExtentZ)
{
	center = inCenter;
	extents = vec3(inExtentX, inExtentY, inExtentZ);
}

BoundingBox::BoundingBox(const Model* model, bool includeChildren)
{
	glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
	if (includeChildren)
	{
		for (int i = 0; i < model->children.size(); i++)
		{
			vec3 childMin = model->children[i]->collectiveBBox->GetMinVector();
			vec3 childMax = model->children[i]->collectiveBBox->GetMaxVector();

			minAABB.x = std::min(minAABB.x, childMin.x);
			minAABB.y = std::min(minAABB.y, childMin.y);
			minAABB.z = std::min(minAABB.z, childMin.z);

			maxAABB.x = std::max(maxAABB.x, childMax.x);
			maxAABB.y = std::max(maxAABB.y, childMax.y);
			maxAABB.z = std::max(maxAABB.z, childMax.z);
		}
	}
	for (Mesh mesh : model->meshes)
	{
		for (auto&& vertex : mesh._vertices)
		{
			minAABB.x = std::min(minAABB.x, vertex.Position.x);
			minAABB.y = std::min(minAABB.y, vertex.Position.y);
			minAABB.z = std::min(minAABB.z, vertex.Position.z);

			maxAABB.x = std::max(maxAABB.x, vertex.Position.x);
			maxAABB.y = std::max(maxAABB.y, vertex.Position.y);
			maxAABB.z = std::max(maxAABB.z, vertex.Position.z);
		}
	}
	center = (maxAABB + minAABB) * 0.5f;
	extents = vec3(maxAABB.x - center.x, maxAABB.y - center.y, maxAABB.z - center.z);
}

const array<vec3, BOX_VERTEX> BoundingBox::getVertice()
{
	std::array<glm::vec3, BOX_VERTEX> vertice;
	vertice[0] = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
	vertice[1] = { center.x + extents.x, center.y - extents.y, center.z - extents.z };
	vertice[2] = { center.x - extents.x, center.y + extents.y, center.z - extents.z };
	vertice[3] = { center.x + extents.x, center.y + extents.y, center.z - extents.z };
	vertice[4] = { center.x - extents.x, center.y - extents.y, center.z + extents.z };
	vertice[5] = { center.x + extents.x, center.y - extents.y, center.z + extents.z };
	vertice[6] = { center.x - extents.x, center.y + extents.y, center.z + extents.z };
	vertice[7] = { center.x + extents.x, center.y + extents.y, center.z + extents.z };
	return vertice;
}

bool BoundingBox::isOnOrForwardPlane(Plane* plane)
{
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	float r = extents.x * std::abs(plane->normal.x) + extents.y * std::abs(plane->normal.y) +
			extents.z * std::abs(plane->normal.z);

	return (-r <= plane->getSignedDistanceToPlane(center));
}

bool BoundingBox::isOnFrustum(vector<Plane*> planes, Entity* inEntity)
{
	//Get global scale thanks to our transform
	vec3 globalCenter = vec3(inEntity->getTRS() * glm::vec4(center, 1.f));

	// Scaled orientation
	glm::vec3 right = inEntity->getRight() * extents.x;
	glm::vec3 up = inEntity->getUp() * extents.y;
	glm::vec3 forward = inEntity->getForward() * extents.z;

	float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

	float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

	float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

	BoundingBox* globalBoundingBox = new BoundingBox(globalCenter, newIi, newIj, newIk);

	bool onFrustum = true;

	for (int i = 0; i < planes.size(); i++)
	{
		if (!globalBoundingBox->isOnOrForwardPlane(planes[i]))
		{
			onFrustum = false;
		}
	}

	delete globalBoundingBox;

	return onFrustum;
}

bool BoundingBox::isOnOrBackwardsPlane(Plane* plane)
{
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	float r = -extents.x * std::abs(plane->normal.x) + -extents.y * std::abs(plane->normal.y) +
		-extents.z * std::abs(plane->normal.z);

	return (-r <= plane->getSignedDistanceToPlane(center));
}

bool BoundingBox::isOutOfFrustum(vector<Plane*> planes, Entity* inEntity)
{
	//Get global scale thanks to our transform
	vec3 globalCenter = vec3(inEntity->getTRS() * glm::vec4(center, 1.f));

	// Scaled orientation
	glm::vec3 right = inEntity->getRight() * extents.x;
	glm::vec3 up = inEntity->getUp() * extents.y;
	glm::vec3 forward = inEntity->getForward() * extents.z;

	float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

	float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

	float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

	BoundingBox* globalBoundingBox = new BoundingBox(globalCenter, newIi, newIj, newIk);

	bool offFrustum = false;

	for (int i = 0; i < planes.size(); i++)
	{
		if (!globalBoundingBox->isOnOrBackwardsPlane(planes[i]))
		{
			offFrustum = true;
		}
	}

	delete globalBoundingBox;

	return offFrustum;
}


vec3 BoundingBox::GetMaxVector()
{
	return vec3(center+extents);
}

vec3 BoundingBox::GetMinVector()
{
	return vec3(center-extents);
}

