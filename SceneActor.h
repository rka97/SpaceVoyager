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
	int GetParameterID(string name);
	virtual Rectangle2D GetEnclosingRectangle();
	virtual void RecalculateBoundaries();
	Model* GetModel();
	virtual bool CheckCollision(SceneActor* );
	virtual float GetEnclosingRadius();
	
protected:
	Model* model;
private:
	int modelView;
	int projection;
	int modelViewNormal;
	int modelViewProjection;
	int modelMatrix;
	int modelNormal;
	int cameraPosition;
	int lightPosition;
	int textureDiffuse1;
	int lightIntensity;
	Rectangle2D enclosingRectangle;
	bool areBoundriesCalculated;
	Box* enclosingBox = nullptr;
};
