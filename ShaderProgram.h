#pragma once
#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
using std::string;
using std::vector;
using std::map;

enum GLSLVariableType
{
	GLSL_VAR_NULL,
	GLSL_VAR_IN,
	GLSL_VAR_UNIFORM,
	GLSL_VAR_END
};

enum ParameterType
{
	SP_NULL,
	SP_BOOL,
	SP_INT,
	SP_FLOAT,
	SP_VEC2,
	SP_VEC3,
	SP_VEC4,
	SP_MAT2,
	SP_MAT3,
	SP_MAT4,
	SP_SAMPLER2D,
	SP_END
};

struct ShaderParameter
{
	unsigned int location;
	unsigned int count;
	ParameterType paramType;
	GLSLVariableType glslVarType;
	ShaderParameter()
	{
		this->location = 0;
		this->count = 0;
		this->paramType = SP_END;
		this->glslVarType = GLSL_VAR_NULL;
	}
	ShaderParameter(unsigned int location, int count, ParameterType type, GLSLVariableType glslType)
	{
		this->location = location;
		this->count = count;
		this->paramType = type;
		this->glslVarType = glslType;
	}
};

class ShaderProgram
{
	friend class Material;
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
	/* Parameter functions. */
	bool AddParameter(string parameterName, unsigned int location, unsigned int count, ParameterType type, GLSLVariableType glslType);
	bool SetParameter(string name, const void* value);
	int	 GetParameterLocation(string name);
private:
	vector<Shader*> shaderList;
	map<string, ShaderParameter> inputParameters; // this map stores parameter name AND parameter type.
	GLint programID;
	bool is_Linked;

	/* Parameter utility functions. */
	void SetBool(int location, int count, const bool* value);
	void SetInt(int location, int count, const GLint* value);
	void SetFloat(int location, int count, const GLfloat* value);
	void SetVec2(int location, int count, const glm::vec2* value);
	void SetVec3(int location, int count, const glm::vec3* value);
	void SetVec4(int location, int count, const glm::vec4* value);
	void SetMat2(int location, int count, const glm::mat2* value);
	void SetMat3(int location, int count, const glm::mat3* value);
	void SetMat4(int location, int count, const glm::mat4* value);
};
