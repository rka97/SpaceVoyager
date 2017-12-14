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
	map<string, Drawable*> sceneActors;
	queue<KeyboardEvent>* inputBuffer;
	bool editorMode;
	void LoadActors();
	void UpdateSceneGameMode();
public:
	Scene();
	~Scene();
	void Initialize();
	void setEditorMode(bool editorMode);
	void UpdateScene();
	bool SetInputBuffer(queue<KeyboardEvent>* inBuff);
	Camera* GetSceneCamera();
	Light* GetSceneLight();
	Drawable* GetActor(string name);
	map<string, Drawable*>* GetSceneActors();
};