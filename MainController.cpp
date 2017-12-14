#include "MainController.h"
#include <iostream>
using namespace std;

queue<KeyboardEvent> MainController::inputBuffer;

long long timeNow;

void MainController::ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyboardEvent ev;
	ev.code = key;
	ev.eventType = action;
	inputBuffer.push(ev);
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
	delete wind;
	delete graphicsController;
	delete sceneController;
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
	glfwSetKeyCallback(wind->GetGLFWPointer(), ProcessInput);
	while (!wind->ShouldClose())
	{
		timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
		sceneController->UpdateScene();
		wind->WindowLoop();
		glfwPollEvents();
		graphicsController->Render();
	}
}
