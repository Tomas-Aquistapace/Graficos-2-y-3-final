#include "Model.h"
#include "modelImporter.h"
#define STB_IMAGE_IMPLEMENTATION


Model::Model(string const& path, Renderer* rend, bool flipUVs /*flipUVs = false*/) : Entity(rend)
{
	_rend = rend;
	parent = NULL;
	individualBBox = NULL;
	collectiveBBox = NULL;
	globalPos = vec3(0);
	modelImporter::loadModel(this, path, rend, flipUVs);
}

Model::Model(Renderer* rend) : Entity(rend)
{
	_rend = rend;
	parent = NULL;
	globalPos = vec3(0);
}

Model::~Model()
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]) delete children[i];
	}
	if (individualBBox) delete individualBBox;
	if (collectiveBBox) delete collectiveBBox;
}

void Model::UpdateTRS()
{
	TRS = translateMat * rotateXMat * rotateYMat * rotateZMat * scaleMat;
	if(parent)
	{
		TRS = parent->TRS * TRS;
	}
}

void Model::Draw(vector<Plane*> planes) // Si recibe solo planos, es frustrum.
{
	// Se updatea el TRS aca por motivos de optimizacion.
	UpdateTRS();
	rend->updateProgram(TRS);
	
	if (!planes.empty() && collectiveBBox->isOnFrustum(planes, this))
	{
		if (individualBBox->isOnFrustum(planes, this))
		{
			for ( unsigned int i = 0; i < meshes.size(); i++)
			{
				meshes[i].Draw();
			}
		}
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->Draw(planes);
		}
	}
}

void Model::Draw(vector<Plane*> planes, Camera* cam) // Si recibe planos y Camara, es BSP
{
	// Se updatea el TRS aca por motivos de optimizacion.
	UpdateTRS();
	rend->updateProgram(TRS);

	bool cameraInBounds = true;
	for (int i = 0; i < planes.size(); i++)
	{
		if (!planes[i]->IsOnPositiveNormal(cam->getPosition()))
		{
			cameraInBounds = false;
		}
	}
	if (cameraInBounds && collectiveBBox->isOnFrustum(planes, this))
	{
		if (!planes.empty())
		{
			if (individualBBox->isOnFrustum(planes, this))
			{
				for (unsigned int i = 0; i < meshes.size(); i++)
				{
					meshes[i].Draw();
				}
			}
			for (int i = 0; i < children.size(); i++)
			{
				children[i]->Draw(planes,cam);
			}
		}
	}
	if (!cameraInBounds && collectiveBBox->isOutOfFrustum(planes, this))
	{
		if (!planes.empty())
		{
			if (individualBBox->isOutOfFrustum(planes, this))
			{
				for (unsigned int i = 0; i < meshes.size(); i++) //here we would check the individual bb
				{
					meshes[i].Draw();
				}
			}
			for (int i = 0; i < children.size(); i++)
			{
				children[i]->Draw(planes, cam);
			}
		}
	}
}

void Model::AddChild(Model* newChild)
{
	children.push_back(newChild);
}

void Model::setPosition(vec3 newPosition) {

	posVec = newPosition;
	translateMat = translate(mat4(1.0f), posVec);
}

void Model::setRotation(vec3 newRot) 
{
	vec3 rot;
	if (newRot.x != rotVec.x)
	{
		rotVec.x = newRot.x;
		rot = vec3(1.0f, 0.0f, 0.0f);
		rotateXMat = rotate(mat4(1.0f), glm::radians(newRot.x), rot);
	}

	if (newRot.y != rotVec.y)
	{
		rotVec.y = newRot.y;
		rot = vec3(0.0f, 1.0f, 0.0f);
		rotateYMat = rotate(mat4(1.0f), glm::radians(newRot.y), rot);
	}

	if (newRot.z != rotVec.z)
	{
		rotVec.z = newRot.z;
		rot = vec3(0.0f, 0.0f, 1.0f);
		rotateZMat = rotate(mat4(1.0f), glm::radians(newRot.z), rot);
	}
}

void Model::setScale(vec3 newScale)
{
	scaleVec = newScale;
	scaleMat = scale(mat4(1.0f), scaleVec);
}
