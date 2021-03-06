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
	string path;
	string type;
};

class Material
{
public:
	Material(string name, ShaderProgram* prog);
	Material(string name, ShaderProgram* prog, vector<Texture> textures);
	Material(const Material& mat);
	~Material();
	bool Initialize();
	bool AddTexture(string path, string type);
	bool AddTexture(Texture& texToAdd);
	void SetTextures(vector<Texture>& tex);
	void ActivateMaterial();
	void BindTextures();
	bool SetParameterValue(int id, void* parameterValue);
	int	GetParameterLocation(string name);
	int	GetParameterID(string name);
protected:
	struct ShaderParameterData {
		string name;
		void* data;
		ShaderParameterData(string name) {
			this->name = name;
			data = nullptr;
		}
		ShaderParameterData() {
			data = nullptr;
		}
	};
	bool SetParameterValue(string parameterName, void* parameter);
	ShaderParameterData* GetShaderParameter(string parameterName);

	string name;
	ShaderProgram* shaderProgram; // is a pointer because many materials can share the same shader program.
	vector<Texture> textures; // is not a pointer because it is expected that materials will not share the same shader program.
	//map<string, void*> shaderProgramParameters; // a string and a pointer to the data.
	vector<ShaderParameterData> shaderParameters;
};