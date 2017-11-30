#pragma once
#include "ShaderProgram.h"
#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

struct Texture {
	unsigned int id;
	string type;
	string path;
};


class Material
{
public:
	Material(string name, ShaderProgram* prog, map<string, void*> shaderProgramParameters, vector<Texture> textures);
	~Material();
	bool InitializeTextures();
	bool SetParameter(string parameterName, void* parameterValue);
private:
	string name;
	ShaderProgram* program;
	vector<Texture> textures;
	map<string, void*> shaderProgramParameters; // a string and a pointer to the data.
};