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
	ShaderProgram* healthBarShader = new ShaderProgram();
	healthBarShader->Initialize();
	healthBarShader->AddAndCompileShader("Shaders\\health_bar.vert", 'v');
	healthBarShader->AddAndCompileShader("Shaders\\health_bar.frag", 'f');
	healthBarShader->LinkProgram();
	healthBarShader->AddParameter("in_position", 0, 1, SP_VEC3, GLSL_VAR_IN);
	healthBarShader->AddParameter("ModelView", 0, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	healthBarShader->AddParameter("Projection", 1, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	healthBarShader->AddParameter("ModelViewNormal", 2, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	healthBarShader->AddParameter("ModelViewProjection", 3, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	healthBarShader->AddParameter("Model", 4, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	healthBarShader->AddParameter("ModelNormal", 5, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	healthBarShader->AddParameter("health", 6, 1, SP_INT, GLSL_VAR_UNIFORM);
	healthBarShader->AddParameter("maxHealth", 7, 1, SP_INT, GLSL_VAR_UNIFORM);
	shaderPrograms["HealthBar"] = healthBarShader;

	ShaderProgram* skyShader = new ShaderProgram();
	skyShader->Initialize();
	skyShader->AddAndCompileShader("Shaders\\sky.vert", 'v');
	skyShader->AddAndCompileShader("Shaders\\sky.frag", 'f');
	skyShader->LinkProgram();
	skyShader->AddParameter("in_position", 0, 1, SP_VEC3, GLSL_VAR_IN);
	skyShader->AddParameter("ModelView", 0, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	skyShader->AddParameter("Projection", 1, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	skyShader->AddParameter("ModelViewNormal", 2, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	skyShader->AddParameter("ModelViewProjection", 3, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	skyShader->AddParameter("Model", 4, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	skyShader->AddParameter("ModelNormal", 5, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	skyShader->AddParameter("iTime", 6, 1, SP_FLOAT, GLSL_VAR_UNIFORM);
	skyShader->AddParameter("iResolution", 7, 1, SP_VEC2, GLSL_VAR_UNIFORM);
	shaderPrograms["Sky"] = skyShader;

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

	ShaderProgram* bulletShader = new ShaderProgram();
	bulletShader->Initialize();
	bulletShader->AddAndCompileShader("Shaders\\bullet.vert", 'v');
	bulletShader->AddAndCompileShader("Shaders\\bullet.frag", 'f');
	bulletShader->LinkProgram();
	
	bulletShader->AddParameter("View", 0, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	bulletShader->AddParameter("Projection", 1, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	bulletShader->AddParameter("Size", 2, 1, SP_VEC2, GLSL_VAR_UNIFORM);
	bulletShader->AddParameter("Angle", 3, 1, SP_FLOAT, GLSL_VAR_UNIFORM);

	bulletShader->AddParameter("InnerRadius", 4, 1, SP_FLOAT, GLSL_VAR_UNIFORM);
	bulletShader->AddParameter("MiddleRadius", 5, 1, SP_FLOAT, GLSL_VAR_UNIFORM);
	bulletShader->AddParameter("OuterRadius", 6, 1, SP_FLOAT, GLSL_VAR_UNIFORM);
	bulletShader->AddParameter("InnerColor", 7, 1, SP_VEC4, GLSL_VAR_UNIFORM);
	bulletShader->AddParameter("MiddleColor", 8, 1, SP_VEC4, GLSL_VAR_UNIFORM);
	bulletShader->AddParameter("OuterColor", 9, 1, SP_VEC4, GLSL_VAR_UNIFORM);

	bulletShader->AddParameter("in_position", 0, 1, SP_VEC3, GLSL_VAR_IN);
	bulletShader->AddParameter("Center_worldspace", 1, 1, SP_VEC3, GLSL_VAR_IN);

	shaderPrograms["Bullet"] = bulletShader;
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
	vec3* materialAmbient = new vec3(0.6f);
	vec3* materialDiffuse = new vec3(0.6f);
	vec3* materialSpecular = new vec3(0.2f);
	vec3* materialShininess = new vec3(32.0f);
	
	shipMaterial->SetParameterValue(shipMaterial->GetParameterID("material.Ka"), materialAmbient);
	shipMaterial->SetParameterValue(shipMaterial->GetParameterID("material.Kd"), materialDiffuse);
	shipMaterial->SetParameterValue(shipMaterial->GetParameterID("material.Ks"), materialSpecular);
	shipMaterial->SetParameterValue(shipMaterial->GetParameterID("material.Shininess"), materialShininess);
	createdParameters.push_back(materialAmbient);
	createdParameters.push_back(materialDiffuse);
	createdParameters.push_back(materialSpecular);
	createdParameters.push_back(materialShininess);

	materials["ToonShip"] = shipMaterial;

	Material* planetMaterial = new Material("ToonPlanet", shaderPrograms[progName]);
	planetMaterial->Initialize();
	materialAmbient = new vec3(0.3f);
	materialDiffuse = new vec3(0.6f);
	materialSpecular = new vec3(0.2f);
	materialShininess = new vec3(32.0f);
	planetMaterial->SetParameterValue(planetMaterial->GetParameterID("material.Ka"), materialAmbient);
	planetMaterial->SetParameterValue(planetMaterial->GetParameterID("material.Kd"), materialDiffuse);
	planetMaterial->SetParameterValue(planetMaterial->GetParameterID("material.Ks"), materialSpecular);
	planetMaterial->SetParameterValue(planetMaterial->GetParameterID("material.Shininess"), materialShininess);
	materials["ToonPlanet"] = planetMaterial;

	createdParameters.push_back(materialAmbient);
	createdParameters.push_back(materialDiffuse);
	createdParameters.push_back(materialSpecular);
	createdParameters.push_back(materialShininess);

	Material* skyMaterial = new Material("SkyMaterial", shaderPrograms["Sky"]);
	skyMaterial->Initialize();
	materials["SkyMaterial"] = skyMaterial;

	Material* bulletMaterial = new Material("BulletProgram", shaderPrograms["Bullet"]);
	bulletMaterial->Initialize();
	materials["BulletMaterial"] = bulletMaterial;

	Material* healthBarMaterial = new Material("HealthBar", shaderPrograms["HealthBar"]);
	healthBarMaterial->Initialize();
	materials["HealthBarMaterial"] = healthBarMaterial;

	Material* shieldBarMaterial = new Material("ShieldBar", shaderPrograms["HealthBar"]);
	healthBarMaterial->Initialize();
	materials["ShieldBarMaterial"] = healthBarMaterial;
}

void SceneGraphicsInformation::LoadModels()
{
	string materialName = "ToonShip";
	if (materials.find(materialName) == materials.end())
	{
		cout << "Error in SceneGraphicsInformation::LoadModels(): trying to use material [" << materialName << "] which is not yet set!\n";
		return;
	}
	string planetMatName = "ToonPlanet";
	if (materials.find(planetMatName) == materials.end())
	{
		cout << "Error in SceneGraphicsInformation::LoadModels(): trying to use material [" << planetMatName << "] which is not yet set!\n";
		return;
	}

	/*
	Model* planetModel = new Model("Planet", "models/planet/planet.obj", materials[planetMatName]);
	planetModel->Initialize();
	models["Planet"] = planetModel;
	*/


	Model* startscreenModel = new Model("Start", "models/StartGame/Start Game.obj", materials[materialName]);
	startscreenModel->Initialize();
	models["Start"] = startscreenModel;

	Model* gameoverModel = new Model("GameOver", "models/gameOveer/gameOver.obj", materials[materialName]);
	gameoverModel->Initialize();
	models["GameOver"] = gameoverModel;

	Model* shipModel = new Model("Imperial", "models/trident/trident.obj", materials[materialName]);
	shipModel->Initialize(); // actually loads the model.
	models["Imperial"] = shipModel;

	Model* saucerModel = new Model("Saucer", "models/saucer/Low_poly_UFO.obj", materials[planetMatName]);
	saucerModel->Initialize();
	models["Saucer"] = saucerModel;

	/*
	Model* fighterModel = new Model("AK5", "models/SciFi/SciFi.obj", materials[planetMatName]);
	fighterModel->Initialize();
	models["Fighter"] = fighterModel;

	Model* rockModel = new Model("Rock", "models/meteor/meteor.obj", materials[planetMatName], false);
	rockModel->Initialize();
	models["Rock"] = rockModel;

	Model* earthModel = new Model("Earth", "models/earth/earth.obj", materials[planetMatName]);
	earthModel->Initialize();
	models["Earth"] = earthModel;
	*/
	BulletModel* bulletModel = new BulletModel("Bullet", materials["BulletMaterial"]);
	models["Bullet"] = bulletModel;

	Model* skyModel = new Model("Sky", "models/skyplane/skyplane_2.obj", materials["SkyMaterial"], false);
	skyModel->Initialize();
	models["Sky"] = skyModel;

	Model* healthBarModel = new Model("HealthBar", "models/skyplane/skyplane_2.obj", materials["HealthBarMaterial"], false);
	healthBarModel->Initialize();
	models["HealthBar"] = healthBarModel;

	Model* shieldBarModel = new Model("ShieldBar", "models/skyplane/skyplane_2.obj", materials["ShieldBarMaterial"], false);
	shieldBarModel->Initialize();
	models["ShieldBar"] = shieldBarModel;
}
