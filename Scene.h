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
#include "BulletsController.h"

class Scene
{
	Camera* camera;
	Light* mainLight;
	SceneGraphicsInformation sceneGraphicsInfo;
	vector<Drawable*> sceneActors;
	void IntialLoad();
	queue<KeyboardEvent>* inputBuffer;
	MouseChange* mouseListener;
	int keyStates[256];
public:
	Scene();
	~Scene();
	void FinalLoad();
	void LoadActors();
	void Initialize();
	void PlayerDeathScene();
	bool UpdatePhysics();
	bool UpdateScene();
	bool SetInputBuffer(queue<KeyboardEvent>* inBuff);
	bool SetMouseListener(MouseChange* chg);
	Camera* GetSceneCamera();
	Light* GetSceneLight();
	vector<Drawable*> GetSceneActors();
	void UpdateKeyboardInput();
	void UpdateMouseInput();
};
