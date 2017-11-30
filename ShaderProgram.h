#pragma once
#include <string>
#include <vector>
#include "Shader.h"
using std::string;
using std::vector;

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	bool Initialize();
	// @type: 'v', vertex shader. 'f', fragment shader.
	bool AddAndCompileShader(string path, char type);
	// Links the shaders in shaderList with the program.
	bool LinkProgram();
	bool UseProgram();
	// Removes a shader with a certain path.
	void RemoveShader(string path);
	// Returns the program information log.
	string GetProgramInfoLog();

private:
	vector<Shader*> shaderList;
	GLint programID;
	bool is_Linked;
};
