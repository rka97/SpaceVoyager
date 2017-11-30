#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "Window.h"
#include "Graphics.h"
class MainController
{
	Window* wind;
	Graphics* graphicsController;
public:
	MainController();
	~MainController();

	bool Initialize();
	void MainLoop();
};

