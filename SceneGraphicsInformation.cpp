#include "SceneGraphicsInformation.h"
#include <glm\glm.hpp>
#include <iostream>
using std::cout;
using glm::vec3;

SceneGraphicsInformation::~SceneGraphicsInformation()
{
	// delete all the parameter values we created.
	for (vector<void*>::iterator it = createdParameters.begin(); it != createdParameters.end(); it++)
	{
		if ((*it) != nullptr)
			delete (*it);
	}
	// delete all the models
	for (map<string, Model*>::iterator it = models.begin(); it != models.end(); it++)
	{
		if (it->second != nullptr)
			delete it->second;
		it->second = nullptr;
	}
	// delete all the materials
	for (map<string, Material*>::iterator it = materials.begin(); it != materials.end(); it++)
	{
		if (it->second != nullptr)
			delete it->second;
		it->second = nullptr;
	}
	// delete all the models
	for (map<string, ShaderProgram*>::iterator it = shaderPrograms.begin(); it != shaderPrograms.end(); it++)
	{
		if (it->second != nullptr)
			delete it->second;
		it->second = nullptr;
	}
}

void SceneGraphicsInformation::Initialize()
{
	LoadShaderPrograms();
	LoadMaterials();
	LoadModels();
}

Model * SceneGraphicsInformation::GetModel(string modelName)
{
	if (models.find(modelName) == models.end())
	{
		cout << "Error in SceneGraphicsInformation::GetModel: could not find model " << modelName << " make sure it is properly loaded.\n";
		return nullptr;
	}
	return models[modelName];
}

void SceneGraphicsInformation::LoadShaderPrograms()
{
	ShaderProgram* toonShader = new ShaderProgram();
	toonShader->Initialize();
	toonShader->AddAndCompileShader("Shaders\\toon.vert", 'v');
	toonShader->AddAndCompileShader("Shaders\\toon.frag", 'f');
	toonShader->LinkProgram();
	toonShader->AddParameter("ModelView", 0, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("Projection", 1, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("ModelViewNormal", 2, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("ModelViewProjection", 3, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("Model", 4, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("ModelNormal", 5, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("cameraPosition", 6, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("texture_diffuse1", 12, 1, SP_SAMPLER2D, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("light.position", 13, 1, SP_VEC4, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("light.intensity", 14, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("material.Ka", 15, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("material.Kd", 16, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("material.Ks", 17, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("material.Shininess", 18, 1, SP_FLOAT, GLSL_VAR_UNIFORM);
	toonShader->AddParameter("in_position", 0, 1, SP_VEC3, GLSL_VAR_IN);
	toonShader->AddParameter("in_normal", 1, 1, SP_VEC3, GLSL_VAR_IN);
	toonShader->AddParameter("in_texCoordinates", 2, 1, SP_VEC2, GLSL_VAR_IN);
	shaderPrograms["Toon"] = toonShader;
}

void SceneGraphicsInformation::LoadMaterials()
{
	string progName = "Toon";
	if (shaderPrograms.find(progName) == shaderPrograms.end())
	{
		cout << "Error in SceneGraphicsInformation::LoadMaterials(): trying to use a shader program not yet set!\n";
		return;
	}
	Material* shipMaterial = new Material("ToonShip", shaderPrograms[progName]);
	shipMaterial->Initialize();
	vec3* materialAmbient = new vec3(0.3f);
	vec3* materialDiffuse = new vec3(0.7f);
	vec3* materialSpecular = new vec3(0.2f);
	vec3* materialShininess = new vec3(32.0f);
	shipMaterial->SetParameterValue("material.Ka", materialAmbient);
	shipMaterial->SetParameterValue("material.Kd", materialDiffuse);
	shipMaterial->SetParameterValue("material.Ks", materialSpecular);
	shipMaterial->SetParameterValue("material.Shininess", materialShininess);
	materials["ToonShip"] = shipMaterial;

	createdParameters.push_back(materialAmbient);
	createdParameters.push_back(materialDiffuse);
	createdParameters.push_back(materialSpecular);
	createdParameters.push_back(materialShininess);
}

void SceneGraphicsInformation::LoadModels()
{
	string materialName = "ToonShip";
	if (materials.find(materialName) == materials.end())
	{
		cout << "Error in SceneGraphicsInformation::LoadModels(): trying to use material [" << materialName << "] which is not yet set!\n";
		return;
	}
	Model* shipModel = new Model("Imperial", "models/corvette/Corvette-F3.obj", materials[materialName], false);
	shipModel->Initialize(); // actually loads the model.
	models["Imperial"] = shipModel;

	Model* rockModel = new Model("Rock", "models/rock/rock.obj", materials[materialName], false);
	rockModel->Initialize();
	models["Rock"] = rockModel;
}
