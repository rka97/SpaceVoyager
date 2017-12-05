#pragma once
#include "Camera.h"
#include "Model.h"
#include "Material.h"
#include "Light.h"
#include "SceneActor.h"
#include "SceneGraphicsInformation.h"
#include <queue>
#include <map>

class Scene
{
	Camera* camera;
	Light* mainLight;
	SceneGraphicsInformation sceneGraphicsInfo;
	map<string, SceneActor*> sceneActors;
	queue<int>* inputBuffer;
	void LoadActors();
public:
	~Scene();
	void Initialize();
	void UpdateScene();
	bool SetInputBuffer(queue<int>* inBuff);
	Camera* GetSceneCamera();
	Light* GetSceneLight();
	SceneActor* GetActor(string name);
};