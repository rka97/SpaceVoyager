#include "Scene.h"
#include <iostream>
using namespace std;

void Scene::LoadActors()
{
	SceneActor* shipActor = new SceneActor();
	shipActor->SetModel(sceneGraphicsInfo.GetModel("Imperial"));
	sceneActors["Player"] = shipActor;
	SceneActor* rockActor = new SceneActor();
	rockActor->SetModel(sceneGraphicsInfo.GetModel("Rock"));
	sceneActors["Rock"] = rockActor;
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
		if (it->second != nullptr)
			delete it->second;
	}
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
	camera->ZoomIn(glm::radians(40.0f));

	mainLight->SetPosition(glm::vec3(0, 1, 0));

	SceneActor* playerShip = GetActor("Player");
	playerShip->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	playerShip->SetScale(vec3(0.004f));
	playerShip->RotateAroundRight(glm::radians(90.0f));
	playerShip->SetPosition(vec3(0.0f, 0.0f, 0.0f));
}

void Scene::setEditorMode(bool editorMode)
{
	this->editorMode = editorMode;
}


void Scene::UpdateSceneGameMode()
{
	while (!inputBuffer->empty())
	{
		int currKey = inputBuffer->front();
		inputBuffer->pop();

		switch (currKey)
		{
		case GLFW_KEY_Q:
			editorMode = !editorMode;
			return;
			break;
		case GLFW_KEY_F:
			camera->Move(camera->Forward());
			break;
		case GLFW_KEY_R:
			camera->Move(camera->Backward());
			break;
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
		//mainLight->SetPosition(camera->Position());
		vec3 lightPos = mainLight->GetLightPosition();
		cout << "current Light Position = (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ").\n";
	}
}

void Scene::UpdateSceneEditMode()
{

	static map<string, SceneActor*>::iterator it = sceneActors.begin();
	if (it->second != nullptr)
	{
		mainLight->SetPosition(sceneActors["Player"]->Position() + vec3(0.0f, 120.0f, 0.0f));
	}
	while (!inputBuffer->empty())
	{
		int currKey = inputBuffer->front();
		inputBuffer->pop();

		switch (currKey)
		{
		case GLFW_KEY_Q:
			editorMode = !editorMode;
			return;
			break;
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
		case GLFW_KEY_ENTER:
			if (it != sceneActors.end())
			{
				vec3 actorPos = it->second->Position();
				cout << "Actor " << it->first << " position = (" << actorPos.x << ", " << actorPos.y << ", " << actorPos.z << ").\n";
				it++;
			}
			break;
		case GLFW_KEY_UP:
			if (it != sceneActors.end())
			{
				it->second->Move(camera->Up());
				mainLight->Move(camera->Up());
			}
			break;
		case GLFW_KEY_RIGHT:
			if (it != sceneActors.end())
			{
				it->second->Move(camera->Right());
				mainLight->Move(camera->Right());
			}
			break;
		case GLFW_KEY_LEFT:
			if (it != sceneActors.end())
			{
				it->second->Move(camera->Left());
				mainLight->Move(camera->Left());
			}
			break;
		case GLFW_KEY_DOWN:
			if (it != sceneActors.end())
			{
				it->second->Move(camera->Down());
				mainLight->Move(camera->Down());
			}
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


void Scene::UpdateScene()
{
	if (!editorMode)
		UpdateSceneGameMode();
	else
		UpdateSceneEditMode();
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

SceneActor * Scene::GetActor(string name)
{
	if (sceneActors.find(name) == sceneActors.end())
	{
		cout << "Error in Scene::GetActor: could not find actor with name " << name << "!\n";
		return nullptr;
	}
	return sceneActors[name];
}
