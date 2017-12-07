#include "Scene.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

bool SHIP_UP = false;
bool SHIP_DOWN = false;
bool SHIP_LEFT = false;
bool SHIP_RIGHT = false;

SceneActor* playerShip;

void Scene::LoadActors()
{
	SceneActor* shipActor = new SceneActor();
	shipActor->SetModel(sceneGraphicsInfo.GetModel("Imperial"));
	sceneActors["Player"] = shipActor;
}


void Scene::Initialize()
{
	camera = new Camera();
	mainLight = new Light(true);
	sceneGraphicsInfo.Initialize();
	LoadActors();

	/* put the camera at the positive z-axis */;
	camera->SetPosition(glm::vec3(0, 0, 20));
	/* turn the camera back to the origin */
	camera->RotateAroundUp(glm::radians(180.0f));
	camera->ZoomOut(glm::radians(50.0f));

	mainLight->SetPosition(glm::vec3(0, 0, -1.0f));
	
	srand(time(0));

	playerShip = GetActor("Player");
	if (playerShip != nullptr)
	{
		playerShip->SetPosition(vec3(0.0f, 0.0f, 0.0f));
		playerShip->SetScale(vec3(2.0f));
		playerShip->RotateAroundRight(glm::radians(90.0f));
		playerShip->RotateAroundUp(glm::radians(180.0f));
		playerShip->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	}
}


Scene::Scene()
{
	camera = nullptr;
	mainLight = nullptr;
	inputBuffer = nullptr;
	editorMode = false;
}

Scene::~Scene()
{
	if (camera != nullptr)
		delete camera;
	if (mainLight != nullptr)
		delete mainLight;
	for (std::map<string, SceneActor*>::iterator it = sceneActors.begin(); it != sceneActors.end(); it++)
	{
		vec3 pos = it->second->Position();
		cout << "actor " << it->first << " has final position (" << pos.x << ", " << pos.y << ", " << pos.z << ").\n";
		if (it->second != nullptr)
			delete it->second;
	}
}

void Scene::setEditorMode(bool editorMode)
{
	this->editorMode = editorMode;
}


void Scene::UpdateSceneGameMode()
{
	while (!inputBuffer->empty())
	{
		KeyboardEvent e = inputBuffer->front();
		inputBuffer->pop();

		switch (e.code)
		{
		case GLFW_KEY_F:
			camera->Move(camera->Forward());
			break;
		case GLFW_KEY_R:
			camera->Move(camera->Backward());
			break;
		case GLFW_KEY_W:
			SHIP_UP = (e.eventType == GLFW_PRESS || e.eventType == GLFW_REPEAT);
			break;
		case GLFW_KEY_A:
			SHIP_LEFT = (e.eventType == GLFW_PRESS || e.eventType == GLFW_REPEAT);
			break;
		case GLFW_KEY_D:
			SHIP_RIGHT = (e.eventType == GLFW_PRESS || e.eventType == GLFW_REPEAT);
			break;
		case GLFW_KEY_S:
			SHIP_DOWN = (e.eventType == GLFW_PRESS || e.eventType == GLFW_REPEAT);
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
	if (SHIP_UP) {
		playerShip->Move(glm::vec3(0,0.2,0));
	}
	if (SHIP_DOWN) {
		playerShip->Move(glm::vec3(0, -0.2, 0));
	}
	if (SHIP_LEFT) {
		playerShip->Move(glm::vec3(-0.2, 0, 0));
	}
	if (SHIP_RIGHT) {
		playerShip->Move(glm::vec3(0.2, 0, 0));
	}
}

void Scene::UpdateScene()
{
	UpdateSceneGameMode();
}

bool Scene::SetInputBuffer(queue<KeyboardEvent>* inBuff)
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

SceneActor * Scene::GetActor(string name)
{
	if (sceneActors.find(name) == sceneActors.end())
	{
		cout << "Error in Scene::GetActor: could not find actor with name " << name << "!\n";
		return nullptr;
	}
	return sceneActors[name];
}

map<string, SceneActor*>* Scene::GetSceneActors()
{
	return &sceneActors;
}
