#include "MainController.h"
#include <iostream>
using namespace std;

queue<KeyboardEvent> MainController::inputBuffer;
MouseChange MainController::mouseListener;
long long timeNow;

void MainController::ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyboardEvent ev;
	ev.code = key;
	ev.eventType = action;
	inputBuffer.push(ev);
}

void MainController::ProcessCursorInput(GLFWwindow * window, double xpos, double ypos)
{
	mouseListener.prevPosition = mouseListener.position;
	mouseListener.position = vec2(xpos, ypos);
	mouseListener.changed = true;
}

MainController::MainController()
{
	this->wind = new Window(40, 40, 1024, 684, "Space Voyager");
	graphicsController = new Graphics();
	this->sceneController = new Scene();
	sceneController->SetInputBuffer(&inputBuffer);
	sceneController->SetMouseListener(&this->mouseListener);
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
	glfwSetCursorPosCallback(wind->GetGLFWPointer(), ProcessCursorInput);
	
	while (!wind->ShouldClose() && glfwGetKey(wind->GetGLFWPointer(), GLFW_KEY_ENTER) != GLFW_PRESS)
	{
		timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
		wind->WindowLoop();
		graphicsController->Render();
	}
	sceneController->LoadActors();
	int timer = -1;
	while (!wind->ShouldClose() && timer != 0)
	{
		timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
		wind->WindowLoop();
		if (!sceneController->UpdateScene())
		{
			if (timer < 0)
				timer = 400; // find a way to properly set this
		}
		timer--;
		graphicsController->Render();
	}
	sceneController->FinalLoad();
	while (!wind->ShouldClose())
	{
		timeNow = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
		wind->WindowLoop();
		glfwPollEvents();
		graphicsController->Render();
	}
}
