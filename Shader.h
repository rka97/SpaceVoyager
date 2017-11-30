#pragma once
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
using std::string;
using std::vector;

class Shader
{
public:
	Shader(string path, char type);
	~Shader();
	bool Initialize();
	bool LoadTextFile();
	bool Compile();
	void DeleteCompiledObject();
	const GLuint GetShaderID() const;

private:
	string filePath;
	string sourceCode;
	string GetTypeName();
	string GetInfoLog();
	char type;
	GLuint shaderID;
	bool active;
};