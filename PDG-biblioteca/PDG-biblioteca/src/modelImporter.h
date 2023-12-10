#pragma once
#include "Exports.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Model.h"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);

class AQUISTAPACE_API modelImporter
{
private:
	static void processNode(aiNode* node, Model* targetParent, mat4 accTransform, const aiScene* scene,Renderer* rend);
	static Mesh processMesh(Model* model, aiMesh* mesh, const aiScene* scene);
	static vector<meshTexture> loadModelMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

public:
	static void loadModel(Model* model, string const& path, Renderer* rend, bool flipUVs);
};

