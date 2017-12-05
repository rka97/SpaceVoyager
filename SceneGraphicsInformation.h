#pragma once
#include "Model.h"
#include "Material.h"
#include <map>
#include <string>
using std::map;
using std::string;

class SceneGraphicsInformation
{
private:
	/* To-do: The next three should read from files instead of being hard-coded. */
	void LoadShaderPrograms();
	void LoadMaterials();
	void LoadModels();
	map<string, Model*> models;
	map<string, Material*> materials;
	map<string, ShaderProgram*> shaderPrograms;
	// stores all the dynamically allocated parameters we created. Doesn't store their information (as of now, it is unneeded).
	// we only want to delete them at the end.
	vector<void*> createdParameters; 
public:
	~SceneGraphicsInformation();
	void Initialize();
	Model* GetModel(string modelName);
};