#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class Model
{
public:
	Model(const string& name, const string& path, Material* mat, bool gamma = false);
	~Model();
	void Draw();
	void Initialize();
	bool SetParameterValue(string parameterName, void* parameterValue);
private:
	void LoadModel();
	void ProcessNode(aiNode* node, const aiScene* scene);
	void FindAdjacencies(aiMesh* mesh, vector<unsigned int>& indices);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

	/* Data. */
	string name;
	string path;
	string directory;

	vector<Mesh> meshes;
	Material* material;
	vector<Material*> childMaterials;
	map<string, Texture> loadedTextures; // maps all the loaded textures so far-- avoids reloading stuff.

	bool gammaCorrection;
};