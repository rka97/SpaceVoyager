#include "SceneActor.h"
#include <iostream>
using std::cout;

SceneActor::SceneActor(vec3 position, vec3 scale) : SceneObject(position, scale)
{
	model = nullptr;
	areBoundriesCalculated = false;
}

float norm2D(vec2 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

bool SceneActor::CheckCollision(SceneActor* SA)
{
	Rectangle2D Me = this->GetEnclosingRectangle();
	Rectangle2D Other = SA->GetEnclosingRectangle();
	return areColliding(Me, Other);
}
float SceneActor::GetEnclosingRadius()
{
	float minX = 9999, minY = 9999, maxX = -9999, maxY = -9999;
	for (auto& vertex : enclosingBox->vertices) {
		glm::vec4 v = GetTransformationMatrix() * vec4(vertex, 1);
		if (v.x < minX) minX = v.x;
		else if (v.x > maxX) maxX = v.x;
		if (v.y < minY) minY = v.y;
		else if (v.y > maxY) maxY = v.y;
	}
	if (maxX - minX > maxY - minY)
		return (maxX - minX)/2;
	else
		return (maxY - minY)/2;
}

bool SceneActor::SetModel(Model* actorModel)
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
//	Projection = glm::ortho(0.f, 400.f, 0.f, 400.f, -1.f, 1.f);

	mat4 ModelViewProjection = Projection * ModelView;
	
	SetParameterValue(modelMatrix, &Model);
	SetParameterValue(modelView, &ModelView);
	SetParameterValue(projection, &Projection);
	SetParameterValue(modelNormal, &ModelNormal);
	SetParameterValue(modelViewProjection, &ModelViewProjection);
	SetParameterValue(cameraPosition, &sceneInfo.cameraPosition);
	SetParameterValue(lightPosition, &sceneInfo.lightPosition);
	SetParameterValue(lightIntensity, &sceneInfo.lightIntensity);
	model->Draw();
}

bool SceneActor::SetParameterValue(int id, void * parameterValue)
{
	return model->SetParameterValue(id, parameterValue);
}

int SceneActor::GetParameterID(string name)
{
	return model->GetParameterId(name);
}

Rectangle2D SceneActor::GetEnclosingRectangle()
{
	RecalculateBoundaries();
	return enclosingRectangle;
}

void SceneActor::RecalculateBoundaries()
{
	float minX = 9999, minY = 9999, maxX = -9999, maxY = -9999;
	if (enclosingBox == nullptr) {
		enclosingBox = new Box;
		*enclosingBox = model->GetEnclosingBox();
	}
	auto tf = GetTransformationMatrix();
	for (auto& vertex : enclosingBox->vertices) {
		glm::vec4 v = tf * vec4(vertex, 1);
		if (v.x < minX) minX = v.x;
		else if (v.x > maxX) maxX = v.x;
		if (v.y < minY) minY = v.y;
		else if (v.y > maxY) maxY = v.y;
	}
	enclosingRectangle = Rectangle2D(vec2(minX, minY), vec2(maxX, maxY));
}

Model * SceneActor::GetModel()
{
	return model;
}
