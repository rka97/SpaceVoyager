#pragma once
#include "Camera.h"
#include "Model.h"
#include "Material.h"
#include "Light.h"
#include "SceneActor.h"
#include "SceneGraphicsInformation.h"
#include <queue>
#include <map>
#include "Utility.h"
#include "BulletsController.h"8

class Scene
{
	Camera* camera;
	Light* mainLight;
	SceneGraphicsInformation sceneGraphicsInfo;
	vector<Drawable*> sceneActors;
	queue<KeyboardEvent>* inputBuffer;
	MouseChange* mouseListener;
	int keyStates[256];
	void LoadActors();
public:
	Scene();
	~Scene();
	void Initialize();
	void UpdateScene();
	bool SetInputBuffer(queue<KeyboardEvent>* inBuff);
	bool SetMouseListener(MouseChange* chg);
	Camera* GetSceneCamera();
	Light* GetSceneLight();
	vector<Drawable*> GetSceneActors();
	void UpdateKeyboardInput();
	void UpdateMouseInput();
};