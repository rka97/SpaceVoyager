#include "MainController.h"
#include <iostream>
using namespace std;

void MainController::ProcessInput()
{
	GLFWwindow* window = wind->GetGLFWPointer();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		wind->SetClose(true);
	}
	for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; i++)
	{
		if (glfwGetKey(window, i) == GLFW_PRESS)
		{
			inputBuffer.push(i);
		}
	}
}

MainController::MainController()
{
	this->wind = new Window(40, 40, 1024, 684, "Space Voyager");
	graphicsController = new Graphics();
	this->sceneController = new Scene();
	sceneController->SetInputBuffer(&inputBuffer);
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
	sceneController->Initialize();
	graphicsController->Initialize(1024, 684, sceneController);
	return true;
}

void MainController::MainLoop()
{
	while (!wind->ShouldClose())
	{
		sceneController->UpdateScene();
		wind->WindowLoop();
		ProcessInput();
		graphicsController->Render();
	}
}
