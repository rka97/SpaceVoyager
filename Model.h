#pragma once
#include "Mesh.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class Model
{
public:
	Model(const string& path, bool gamma = false);
	void Draw();
	void Initialize();
private:
	void LoadModel();
	void ProcessNode();
	void ProcessMesh();
	vector<Mesh> meshes;
	vector<Material> materials;
	map<string, Texture> loadedTextures; // maps all the loaded textures so far-- avoids reloading stuff.
	bool gammaCorrection;
};