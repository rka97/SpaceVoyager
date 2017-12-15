#include "SceneActor.h"
#include <iostream>
using std::cout;

SceneActor::SceneActor(vec3 position, vec3 scale) : SceneObject(position, scale)
{
	model = nullptr;
}

bool SceneActor::SetModel(Model * actorModel)
{
	if (actorModel == nullptr) {
		cout << "Error in SceneActor::SetMode: trying to set NULL model.\n";
		return false;
	}
	model = actorModel;
	projection = model->GetParameterId("Projection");
	modelView = model->GetParameterId("ModelView");
	modelViewNormal = model->GetParameterId("ModelViewNormal");
	modelViewProjection = model->GetParameterId("ModelViewProjection");
	modelMatrix = model->GetParameterId("Model");
	modelNormal = model->GetParameterId("ModelNormal");
	cameraPosition = model->GetParameterId("cameraPosition");
	textureDiffuse1 = model->GetParameterId("texture_diffuse1");
	lightPosition = model->GetParameterId("light.position");
	lightIntensity = model->GetParameterId("light.intensity");
	return true;
}

void SceneActor::Draw(SceneInfo& sceneInfo, int numInstances)
{
	if (model == nullptr) {
		cout << "Error in SceneActor::DrawActor: trying to draw actor with NULL model.\n";
		return;
	}
	mat4 Model = GetTransformationMatrix();
	mat4 ModelView = sceneInfo.view * Model;
	mat4 ModelNormal = GetRotationMatrix(); //glm::transpose(glm::inverse(Model));
	mat4 Projection = sceneInfo.projection;
	mat4 ModelViewProjection = Projection * ModelView;
	
	SetParameterValue(modelMatrix, &Model);
	SetParameterValue(modelView, &ModelView);
	SetParameterValue(projection, &Projection);
	SetParameterValue(modelNormal, &ModelNormal);
	SetParameterValue(modelViewProjection, &ModelViewProjection);
	SetParameterValue(cameraPosition, &sceneInfo.view[3]);
	SetParameterValue(lightPosition, &sceneInfo.lightPosition);
	SetParameterValue(lightIntensity, &sceneInfo.lightIntensity);
	model->Draw();
}

bool SceneActor::SetParameterValue(int id, void * parameterValue)
{
	return model->SetParameterValue(id, parameterValue);
}

Model * SceneActor::GetModel()
{
	return model;
}
