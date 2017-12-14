#include "Material.h"
#include <string>
#include <iostream>
using std::cout;

Material::Material(string name, ShaderProgram * prog)
{
	this->name = name;
	this->shaderProgram = prog;
}

Material::Material(string name, ShaderProgram * prog, vector<Texture> textures)
{
	this->name = name;
	this->shaderProgram = prog;
	this->textures = textures;
}

Material::Material(const Material & mat)
{
	this->name = mat.name;
	this->shaderProgram = mat.shaderProgram;
	this->shaderParameters = mat.shaderParameters;
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
	else if (shaderProgram->is_Linked == false)
	{
		cout << "Error in Material::Initialize: shaderProgram is not yet linked!\n";
		return false;
	}
	for (std::vector<ShaderParameter>::const_iterator it = shaderProgram->shaderInputParameters.begin(); it != shaderProgram->shaderInputParameters.end(); it++)
	{
		//shaderProgramParameters[it->first] = NULL;
		shaderParameters.push_back(ShaderParameterData(it->name));
	}
	// Initialize shader program (if not already initialized).
	return true;
}

bool Material::AddTexture(Texture & texToAdd)
{
	textures.push_back(texToAdd);
	return true;
}

void Material::SetTextures(vector<Texture>& tex)
{
	this->textures = tex;
}

void Material::ActivateMaterial()
{
	if (shaderProgram == NULL)
	{
		cout << "Error in Material::ActivateMaterial(): trying to activate NULL shader program.\n";
	}
	shaderProgram->UseProgram();
	this->BindTextures();
	for (int i = 0; i < shaderParameters.size(); i++) {
		if (shaderParameters[i].data != nullptr)
			shaderProgram->SetParameter(i, shaderParameters[i].data);
	}
	/*
	for (map<string, void*>::iterator it = shaderProgramParameters.begin(); it != shaderProgramParameters.end(); it++)
	{
		if (it->second != NULL)
			shaderProgram->SetParameter(it->first, it->second);
	}
	*/
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
		else
			continue;
		// enable these later.
		/*
		else if (name == "texture_specular")
			number = std::to_string(numSpecular++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(numNormalMaps++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(numHeightMaps++); // transfer unsigned int to stream
			*/
		string textureParameterName = name + number;
		int* j;

		/*
		if (shaderProgramParameters[textureParameterName] != NULL)
		{
			j = (int*)shaderProgramParameters[textureParameterName];
			delete j;
		}
		j = new int;
		*j = i;
		*/

		ShaderParameterData* spd = GetShaderParameter(textureParameterName);
		if (spd == nullptr) continue;
		if (spd->data != nullptr) {
			j = (int*)spd->data;
			delete j;
		}
		
		j = new int;
		*j = i;

		// now set the sampler to the correct texture unit
		SetParameterValue(textureParameterName, j);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}

Material::ShaderParameterData* Material::GetShaderParameter(string parameterName) {
	for (auto& par : shaderParameters) {
		if (par.name == parameterName) {
			return &par;
		}
	}
	return nullptr;
}

int	Material::GetParameterID(string parameterName) {
	int i = 0;
	for (auto& par : shaderParameters) {
		if (par.name == parameterName) {
			return i;
		}
		i++;
	}
	return -1;
}

bool Material::SetParameterValue(string parameterName, void* parameterValue) {
	for (auto& par : shaderParameters) {
		if (par.name == parameterName) {
			par.data = parameterValue;
			return true;
		}
	}
	return false;
}

bool Material::SetParameterValue(int id, void* parameterValue)
{
	/*
	if (shaderProgramParameters.find(parameterName) == shaderProgramParameters.end())
	{
		// cout << "Error in Material::SetParameterValue(str, const void*): trying to set material parameter [" << parameterName << "] which is not in the shader program.\n";
		return false;
	}
	else if (parameterValue == NULL)
	{
		// cout << "Error in Material::SetParameterValue(str, const void*): trying to set material parameter to NULL pointer.\n";
		return false;
	}
	else
	{
		shaderProgramParameters[parameterName] = parameterValue;
		return true;
	}
	*/
	if (id >= shaderParameters.size() || id < 0) {
		// cout << "Error in Material::SetParameterValue(str, const void*): trying to set material parameter [" << parameterName << "] which is not in the shader program.\n";
		return false;
	}
	shaderParameters[id].data = parameterValue;
}

int Material::GetParameterLocation(string name)
{
	if (shaderProgram == NULL)
		return -1;
	else
		return shaderProgram->GetParameterLocation(name);
}


