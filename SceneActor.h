#pragma once
#include "SceneObject.h"
#include "Model.h"
#include "Utility.h"
#include "Drawable.h"

class SceneActor : public SceneObject, public Drawable
{
public:
	SceneActor(vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual bool SetModel(Model* actorModel);
	virtual void Draw(SceneInfo& sceneInfo, int numInstances);
	bool SetParameterValue(int id, void* parameterValue);
	Model* GetModel();
protected:
	Model* model;
private:
	static int MODEL_VIEW;
	static int PROJECTION;
	static int MODEL_VIEW_NORMAL;
	static int MODEL_VIEW_PROJECTION;
	static int MODEL;
	static int MODEL_NORMAL;
	static int CAMERA_POSITION;
	static int TEXTURE_DIFFUSE1;
	static int LIGHT;
	static int MATERIAL;
};