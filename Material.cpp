#include "Material.h"
#include <string>
#include <iostream>
using std::cout;
Material::Material(string name, ShaderProgram * prog)
{
}

Material::Material(string name, ShaderProgram * prog, vector<Texture> textures)
{
	this->name = name;
	this->shaderProgram = prog;
	this->textures = textures;
}

Material::~Material()
{
	
}

bool Material::Initialize()
{
	if (shaderProgram == NULL)
	{
		cout << "Error in Material::Initialize: shaderProgram is NULL!\n";
		return false;
	}
	else if (shaderProgram->is_Linked = false)
	{
		cout << "Error in Material::Initialize: shaderProgram is not yet linked!\n";
	}
	for (std::map<string, ShaderParameter>::const_iterator it = shaderProgram->inputParameters.begin(); it != shaderProgram->inputParameters.end(); it++)
	{
		shaderProgramParameters[it->first] = NULL;
	}
	// Initialize shader program (if not already initialized).
	return shaderProgram->Initialize();
}

bool Material::AddTexture(string path, string type)
{
	return false;
}
bool Material::AddTexture(Texture & texToAdd)
{
	textures.push_back(texToAdd);
	return true;
}

void Material::BindTextures()
{
	// bind appropriate textures
	unsigned int numDiffuse = 1;
	unsigned int numSpecular = 1;
	unsigned int numNormalMaps = 1;
	unsigned int numHeightMaps = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(numDiffuse++);
		else if (name == "texture_specular")
			number = std::to_string(numSpecular++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(numNormalMaps++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(numHeightMaps++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		SetParameterValue(name + number, &i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}

bool Material::SetParameterValue(string parameterName, const void * parameterValue)
{
	if (this->shaderProgram == NULL)
	{
		return false;
	}
	else
	{
		return shaderProgram->SetParameter(parameterName, parameterValue);
	}
}

int Material::GetParameterLocation(string name)
{
	if (shaderProgram == NULL)
		return -1;
	else
		return shaderProgram->GetParameterLocation(name);
}


