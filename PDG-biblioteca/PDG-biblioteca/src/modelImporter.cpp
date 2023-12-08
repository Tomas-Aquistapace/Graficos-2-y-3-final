#include "modelImporter.h"
#include "glm/glm.hpp"

string directory;
vector<meshTexture> textures_loaded;

void modelImporter::loadModel(Model* model, string const& path, Renderer* rend, bool flipUVs)
{
	stbi_set_flip_vertically_on_load(flipUVs);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/')); //

	model->name = scene->mRootNode->mName.C_Str();
	//models_Loaded.back()->name = scene->mRootNode->mName.C_Str();
	
	processNode(scene->mRootNode, model, model->getTRS(), scene, rend);

}

void modelImporter::processNode(aiNode* node, Model* targetParent, mat4 accTransform , const aiScene* scene, Renderer* rend)
{
	Model* parent;
	mat4 transform;

	if (node->mParent == NULL)
	{
		parent = targetParent;

		aiVector3D aiPos;
		aiVector3D aiScale;
		aiQuaternion aiRot;
		node->mTransformation.Decompose(aiScale, aiRot, aiPos);

		parent->globalPos = vec3(aiPos.x, aiPos.y, aiPos.z);

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			targetParent->meshes.push_back(processMesh(targetParent, mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			cout << "Processing child " << i << " of parent " << parent->name << endl;
			processNode(node->mChildren[i], parent, parent->getTRS(), scene, rend);
		}
	}
	else
	{
		Model* newModel = new Model(rend);
		newModel->name = node->mName.C_Str();
		targetParent->AddChild(newModel);
		newModel->parent = targetParent;
		//Copy Meshes
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			//cout << "Loading mesh " << i << " of node " << node->mName.C_Str() << " into " << newModel->name << endl;
			newModel->meshes.push_back(processMesh(newModel, mesh, scene));
		}

		//cout << "Parent is " << parent->name << endl;
		parent = newModel;
		cout << "Parent changed to " << parent->name << endl;


		aiVector3D aiPos;
		aiVector3D aiScale;
		aiQuaternion aiRot;
		node->mTransformation.Decompose(aiScale, aiRot, aiPos);
		//cout << "AIPOSGLOBAL :" << aiPos.x << " " << aiPos.y << " " << aiPos.z << endl;
		newModel->globalPos = vec3(aiPos.x, aiPos.y, aiPos.z);
		newModel->setPosition(vec3(0));

		cout << "Node " << parent->name << " processed." << endl;
		cout << "Processing children of " << parent->name << "." << endl;
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			cout << "Processing child" << i << " of parent " << parent->name << endl;
			processNode(node->mChildren[i], parent, parent->getTRS(), scene, rend);
		}
	}
	parent->individualBBox = new BoundingBox(parent, false);
	parent->collectiveBBox = new BoundingBox(parent, true);
}

Mesh modelImporter::processMesh(Model* model, aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<meshTexture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	vector<meshTexture> diffuseMaps = loadModelMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	vector<meshTexture> specularMaps = loadModelMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	if (specularMaps.empty()) model->hasSpecularMaps = false;
	else model->hasSpecularMaps = true;

	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// NORMAL USES HEIGHT INSTEAD OF NORMALS
	vector<meshTexture> normalMaps = loadModelMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// HEIGHT USES AMBIENT INSTEAD OF HEIGHT 
	vector<meshTexture> heightMaps = loadModelMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures, model->hasSpecularMaps, model->_rend);
}

vector<meshTexture> modelImporter::loadModelMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<meshTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		cout << str.C_Str() << endl;
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			meshTexture texture;
			texture.id = TextureFromFile(str.C_Str(), directory, false);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width;
	int height;
	int nrComponents;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		cout << "Texture failed to load at path: " << path << endl;
		stbi_image_free(data);
	}
	return textureID;
}

