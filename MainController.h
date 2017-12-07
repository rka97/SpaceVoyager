#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "Window.h"
#include "Graphics.h"
#include "Scene.h"
#include <queue>
#include "Utility.h"

class MainController
{
public:
	MainController();
	~MainController();

	bool Initialize();
	void MainLoop();

private:
	Window* wind;
	Graphics* graphicsController;
	Scene* sceneController;
	static queue<KeyboardEvent> inputBuffer;
	static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods);
};

