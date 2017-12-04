#pragma once
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include <queue>

class Scene
{
	Camera* camera;
	Light* mainLight;
	vector<Model*> models;
	queue<int>* inputBuffer;
public:
	~Scene();
	void Initialize();
	void UpdateScene();
	bool SetInputBuffer(queue<int>* inBuff);
	Camera* GetSceneCamera();
	Light* GetSceneLight();
};