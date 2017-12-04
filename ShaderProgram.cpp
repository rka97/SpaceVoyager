#include "ShaderProgram.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	if (programID == 0)
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
		is_Linked = true;
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
		if ((*it)->GetPath() == path)
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

bool ShaderProgram::AddParameter(string parameterName, unsigned int location, unsigned int count, ParameterType type, GLSLVariableType glslType)
{
	if (parameterName == "")
	{
		cout << "Error in ShaderProgram::AddParameter: Parameter Name can not be empty.\n";
		return false;
	}
	if (inputParameters.find(parameterName) != inputParameters.end())
	{
		cout << "Error in ShaderProgram::AddParameter: trying to add parameter " << parameterName << " which is already added.\n";
		return false;
	}
	if (type >= SP_END)
	{
		cout << "Error in ShaderProgram::AddParameter: Parameter Type can not be equal to or greater than SP_END.\n";
		return false;
	}
	else if (type == SP_NULL)
	{
		cout << "Error in ShaderProgram::AddParameter: Parameter Type can not be equal to SP_NULL.\n";
		return false;
	}
	if (glslType >= GLSL_VAR_END)
	{
		cout << "Error in ShaderProgram::AddParameter: GLSL Variable Type can not be equal to or greater than GLSL_VAR_END.\n";
		return false;
	}
	else if (glslType == GLSL_VAR_NULL)
	{
		cout << "Error in ShaderProgram::AddParameter: GLSL Variable Type can not be equal to GLSL_VAR_NULL.\n";
		return false;
	}
	ShaderParameter param(location, count, type, glslType);
	inputParameters[parameterName] = param;
	return true;
}

bool ShaderProgram::SetParameter(string name, const void * value)
{
	if (!is_Linked)
	{
		cout << "Error in ShaderProgram::SetParameter: trying to set input parameter without linking first!\n";
		return false;
	}
	if (value == NULL)
	{
		cout << "Error in ShaderProgram::SetParameter: trying to set input parameter to NULL!\n";
		return false;
	}
	map<string, ShaderParameter>::const_iterator it = inputParameters.find(name);
	if (it == inputParameters.end())
	{
		cout << "Error in ShaderProgram::SetParameter: parameter named " << name << " not found in input parameters!\n";
		return false;
	}
	if (it->second.glslVarType == GLSL_VAR_UNIFORM)
	{
		switch (it->second.paramType)
		{
		case SP_BOOL:
			SetBool(it->second.location, it->second.count, (const bool*)value);
			return true;
			break;
		case SP_INT:
			SetInt(it->second.location, it->second.count, (const GLint*)value);
			return true;
			break;
		case SP_FLOAT:
			SetFloat(it->second.location, it->second.count, (const GLfloat*)value);
			return true;
			break;
		case SP_VEC2:
			SetVec2(it->second.location, it->second.count, (const glm::vec2*) value);
			return true;
			break;
		case SP_VEC3:
			SetVec3(it->second.location, it->second.count, (const glm::vec3*) value);
			return true;
			break;
		case SP_VEC4:
			SetVec4(it->second.location, it->second.count, (const glm::vec4*) value);
			return true;
			break;
		case SP_MAT2:
			SetMat2(it->second.location, it->second.count, (const glm::mat2*) value);
			return true;
			break;
		case SP_MAT3:
			SetMat3(it->second.location, it->second.count, (const glm::mat3*) value);
			return true;
			break;
		case SP_MAT4:
			SetMat4(it->second.location, it->second.count, (const glm::mat4*) value);
			return true;
			break;
		case SP_SAMPLER2D:
			SetInt(it->second.location, it->second.count, (const GLint*)value);
			return true;
			break;
		case SP_NULL:
			cout << "Error: Trying to set parameter of type SP_NULL. \n";
			return false;
		case SP_END:
		default:
			cout << "Error: unsupported SP Parameter Type: " << it->second.paramType << "\n";
			return false;
			break;
		}
	}
	else if (it->second.glslVarType == GLSL_VAR_IN)
	{
		cout << "Error in ShaderProgram::SetParameter: GLSL_VAR_IN is not supposed to be passed like this.\n";
		return false;
	}
	else
	{
		cout << "Error in ShaderProgram::SetParameter: GLSL Variable Type unsupported.\n";
		return false;
	}
}

int ShaderProgram::GetParameterLocation(string name)
{
	map<string, ShaderParameter>::const_iterator it = inputParameters.find(name);
	if (it == inputParameters.end())
	{
		cout << "Error in ShaderProgram::GetParameterLocation: parameter named " << name << " not found in input parameters!\n";
		return -1;
	}
	else
	{
		return it->second.location;
	}
}

void ShaderProgram::SetBool(int location, int count, const bool* value)
{
	glProgramUniform1iv(programID, location, count, (const int*)value);
}

void ShaderProgram::SetInt(int location, int count, const GLint* value)
{
	glProgramUniform1iv(programID, location, count, value);
}

void ShaderProgram::SetFloat(int location, int count, const GLfloat* value)
{
	glProgramUniform1fv(programID, location, count, value);
}

void ShaderProgram::SetVec2(int location, int count, const glm::vec2* value)
{
	glProgramUniform2fv(programID, location, count, glm::value_ptr(*value));
}

void ShaderProgram::SetVec3(int location, int count, const glm::vec3* value)
{
	glProgramUniform3fv(programID, location, count, glm::value_ptr(*value));
}

void ShaderProgram::SetVec4(int location, int count, const glm::vec4* value)
{
	glProgramUniform4fv(programID, location, count, glm::value_ptr(*value));
}

void ShaderProgram::SetMat2(int location, int count, const glm::mat2* value)
{
	glProgramUniformMatrix2fv(programID, location, count, GL_FALSE, glm::value_ptr(*value));
}

void ShaderProgram::SetMat3(int location, int count, const glm::mat3* value)
{
	glProgramUniformMatrix3fv(programID, location, count, GL_FALSE, glm::value_ptr(*value));
}

void ShaderProgram::SetMat4(int location, int count, const glm::mat4* value)
{
	glProgramUniformMatrix4fv(programID, location, count, GL_FALSE, glm::value_ptr(*value));
}
