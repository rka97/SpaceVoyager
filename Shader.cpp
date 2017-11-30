#include "Shader.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>
using namespace std;

string Shader::GetTypeName()
{
	if (glIsShader(shaderID))
	{
		GLint type = 0;
		glGetShaderiv(shaderID, GL_SHADER_TYPE, &type); // returns the type parameter from the shader object.
		switch (type)
		{
		case GL_VERTEX_SHADER:
			return "Vertex Shader";
		case GL_FRAGMENT_SHADER:
			return "Fragment Shader";
		case GL_TESS_CONTROL_SHADER:
			return "Tessellation Control Shader";
		case GL_TESS_EVALUATION_SHADER:
			return "Tessellation Evaluation Shader";
		case GL_GEOMETRY_SHADER:
			return "Geometry Shader";
		case GL_COMPUTE_SHADER:
			return "Compute Shader";
		default:
			return "Other Shader";
		}
	}
	return "Invalid Shader";
}

Shader::Shader(string path, char type)
{
	this->filePath = path;
	sourceCode = "";
	shaderID = 0;
	this->type = type;
	active = false;
}

Shader::~Shader()
{
	DeleteCompiledObject();
}

bool Shader::Initialize()
{
	switch (type)
	{
	case 'v':
	case 'V':
		this->shaderID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case 'f':
	case 'F':
		this->shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		cout << "Incorrect input shader type.\n";
		break;
	}
	return (shaderID != 0) && LoadTextFile();
}

bool Shader::LoadTextFile()
{
	sourceCode = "";
	string line;
	ifstream file;
	file.open(filePath.c_str());
	if (file.fail())
	{
		char error[1024];
		strerror_s(error, 1024, errno);
		cout << error << "\n";
		return false;
	}
	while (file.good())
	{
		std::getline(file, line);
		sourceCode += line + '\n';
	}
	file.close();
	return true;
}

bool Shader::Compile()
{
	if (!shaderID)
	{
		cout << "Error: uninitialized shader!\n";
		return false;
	}
	if (glIsShader(shaderID) != GL_TRUE)
	{
		cout << "Error: Shader compilation failed: shader parameter is not a shader object.\n";
		return false;
	}
	if (sourceCode.empty())
	{
		cout << "Error: Shader compilation failed: empty source code!\n";
		return false;
	}
	const GLchar* sourceCodeCStr = sourceCode.c_str();
	glShaderSource(shaderID, 1, &sourceCodeCStr, NULL); // length is NULL: i.e., assume it is null-terminated.
	glCompileShader(shaderID);

	GLint status = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
	{
		active = true;
		return true;
	}
	else
	{
		cout << "Error: Failed to compile shader (" << GetTypeName() << ").\n" << GetInfoLog() << "\n";
		return false;
	}
}

void Shader::DeleteCompiledObject()
{
	if (active)
	{
		glDeleteShader(shaderID);
	}
}

const GLuint Shader::GetShaderID() const
{
	return shaderID;
}

const string Shader::GetPath() const
{
	return filePath;
}

string Shader::GetInfoLog()
{
	if (glIsShader(shaderID))
	{
		GLint logSize = 0;
		GLchar infoLog[1024] = { 0 };
		glGetShaderInfoLog(shaderID, 1024, &logSize, infoLog);
		return string(infoLog);
	}

	return "Invalid Shader.";
}
