#include "ShaderProgram.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

ShaderProgram::ShaderProgram()
{
	programID = 0;
	is_Linked = false;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(programID);
	for (vector<Shader*>::iterator it = shaderList.begin(); it != shaderList.end(); it++)
	{
		delete (*it);
	}
}

bool ShaderProgram::Initialize()
{
	programID = glCreateProgram();
	return (programID == 0);
}

bool ShaderProgram::AddAndCompileShader(string path, char type)
{
	Shader* newShader = new Shader(path, type);
	if (!newShader->Initialize())
	{
		cout << "Could not initialize shader.\n";
		return false;
	}
	if (!newShader->Compile())
	{
		cout << "Could not compile shader.\n";
		return false;
	}
	shaderList.push_back(newShader);
	return true;
}

bool ShaderProgram::LinkProgram()
{
	if (!glIsProgram(programID))
	{
		cout << "ERROR: Shader linking failed: invalid program.\n";
		return false;
	}

	for (vector<Shader*>::const_iterator it = shaderList.begin(); it != shaderList.end(); it++)
	{
		const GLint shaderID = (*it)->GetShaderID();
		if (glIsShader(shaderID) == GL_TRUE)
		{
			glAttachShader(programID, shaderID);
		}
	}

	glLinkProgram(programID);

	for (vector<Shader*>::const_iterator it = shaderList.begin(); it != shaderList.end(); it++)
	{
		const GLint shaderID = (*it)->GetShaderID();
		if (glIsShader(shaderID) == GL_TRUE)
		{
			glDetachShader(programID, shaderID);
		}
	}

	GLint status = 0;

	glGetProgramiv(programID, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Error: Shader linking failed.\n" << GetProgramInfoLog() << "\n";
		return false;
	}
	else
	{
		for (vector<Shader*>::iterator it = shaderList.begin(); it != shaderList.end(); it++)
			(*it)->DeleteCompiledObject();
		return true;
	}
}

bool ShaderProgram::UseProgram()
{
	if (programID != 0 && is_Linked)
	{
		glUseProgram(programID);
	}
	return (programID == 0) && is_Linked;
}

void ShaderProgram::RemoveShader(string path)
{
	for (vector<Shader*>::iterator it = shaderList.begin(); it != shaderList.end(); it++)
	{
		if ((*it)->filePath == path)
		{
			shaderList.erase(it);
		}
	}
}

string ShaderProgram::GetProgramInfoLog()
{
	if (glIsProgram(programID))
	{
		GLint logSize = 0;
		GLchar infoLog[1024] = { 0 };
		glGetProgramInfoLog(programID, 1024, &logSize, infoLog);
		return string(infoLog);
	}

	return "Invalid Program.";
}
