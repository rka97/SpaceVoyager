#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "Window.h"
#include "Graphics.h"
#include "Scene.h"
#include <queue>

class MainController
{
	Window* wind;
	Graphics* graphicsController;
	Scene* sceneController;
	queue<int> inputBuffer;
	void ProcessInput();
public:
	MainController();
	~MainController();

	bool Initialize();
	void MainLoop();
};

