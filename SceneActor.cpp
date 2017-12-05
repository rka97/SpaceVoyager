#include "SceneActor.h"
#include <iostream>
using std::cout;

SceneActor::SceneActor(vec3 position, vec3 scale) : SceneObject(position, scale)
{
	model = nullptr;
}

bool SceneActor::SetModel(Model * actorModel)
{
	if (actorModel != nullptr)
	{
		model = actorModel;
		return true;
	}
	else
	{
		cout << "Error in SceneActor::SetModel: trying to set NULL model.\n";
		return false;
	}
}

void SceneActor::Draw()
{
	if (model != nullptr)
	{
		model->Draw();
	}
	else
	{
		cout << "Error in SceneActor::DrawActor: trying to draw actor with NULL model.\n";
		return;
	}
}

bool SceneActor::SetParameterValue(string parameterName, void * parameterValue)
{
	return model->SetParameterValue(parameterName, parameterValue);
}
