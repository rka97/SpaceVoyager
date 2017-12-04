#include "Scene.h"
#include <iostream>
using namespace std;

Scene::~Scene()
{
	if (camera != nullptr)
		delete camera;
	if (mainLight != nullptr)
		delete mainLight;
}

void Scene::Initialize()
{
	this->camera = new Camera();
	this->mainLight = new Light(true);
}

void Scene::UpdateScene()
{
	while (!inputBuffer->empty())
	{
		int currKey = inputBuffer->front();
		inputBuffer->pop();

		switch (currKey)
		{
		case GLFW_KEY_W:
			camera->Move(camera->Up());
			break;
		case GLFW_KEY_A:
			camera->Move(camera->Right());
			break;
		case GLFW_KEY_D:
			camera->Move(camera->Left());
			break;
		case GLFW_KEY_S:
			camera->Move(camera->Down());
			break;
		case GLFW_KEY_UP:
			mainLight->Move(camera->Up());
			break;
		case GLFW_KEY_RIGHT:
			mainLight->Move(camera->Right());
			break;
		case GLFW_KEY_LEFT:
			mainLight->Move(camera->Left());
			break;
		case GLFW_KEY_DOWN:
			mainLight->Move(camera->Down());
			break;
		case GLFW_KEY_KP_ADD:
			camera->ZoomIn(glm::radians(1.0f));
			break;
		case GLFW_KEY_KP_SUBTRACT:
			camera->ZoomOut(glm::radians(1.0f));
			break;
		default:
			break;
		}
	}
}

bool Scene::SetInputBuffer(queue<int>* inBuff)
{
	if (inBuff != nullptr)
	{
		this->inputBuffer = inBuff;
		return true;
	}
	else
	{
		cout << "Error in Scene::SetInputBuffer: can not set input buffer to NULL.\n";
		return false;
	}
}

Camera * Scene::GetSceneCamera()
{
	return camera;
}

Light * Scene::GetSceneLight()
{
	return mainLight;
}
