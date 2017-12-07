#pragma once
#include "SceneObject.h"
#include "Model.h"

class SceneActor : public SceneObject
{
public:
	SceneActor(vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	bool SetModel(Model* actorModel);
	virtual void Draw();
	bool SetParameterValue(string parameterName, void * parameterValue);
private:
	Model* model;
};