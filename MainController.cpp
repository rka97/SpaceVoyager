#include "MainController.h"
#include <iostream>
using namespace std;

MainController::MainController()
{
	this->wind = new Window(40, 40, 1024, 684, "Space Voyager");
	graphicsController = new Graphics();
}


MainController::~MainController()
{
}

bool MainController::Initialize()
{
	if (!wind->Initialize())
	{
		cout << "Failed to initialize the window.\n";
		return 0;
	}
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return 0;
	}
	graphicsController->Initialize(1024, 684);
	return true;
}

void ProcessInput(Window* wind)
{
	GLFWwindow* window = wind->GetGLFWPointer();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		wind->SetClose(true);
	}
}


void MainController::MainLoop()
{
	while (!wind->ShouldClose())
	{
		wind->WindowLoop();
		ProcessInput(wind);
		graphicsController->Render();
	}
}
