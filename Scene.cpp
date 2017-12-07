#include "Scene.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void Scene::LoadActors()
{
	SceneActor* shipActor = new SceneActor();
	shipActor->SetModel(sceneGraphicsInfo.GetModel("Imperial"));
	sceneActors["Player"] = shipActor;

	SceneActor* rockActor = new SceneActor();
	for (int i = 0; i < 5; i++)
	{
		rockActor = new SceneActor();
		rockActor->SetModel(sceneGraphicsInfo.GetModel("Rock"));
		sceneActors["TempRock" + to_string(i)] = rockActor;
	}

	SceneActor* fighterActor = new SceneActor();
	fighterActor->SetModel(sceneGraphicsInfo.GetModel("Fighter"));
	sceneActors["Fighter1"] = fighterActor;

	for (int i = 0; i < 5; i++)
	{
		fighterActor = new SceneActor();
		fighterActor->SetModel(sceneGraphicsInfo.GetModel("Fighter"));
		sceneActors["TempFighter" + to_string(i)] = fighterActor;
	}

	SceneActor* saucerActor = new SceneActor();
	saucerActor->SetModel(sceneGraphicsInfo.GetModel("Saucer"));
	sceneActors["Saucer1"] = saucerActor;

	/*
	SceneActor* skyActor = new SceneActor();
	skyActor->SetModel(sceneGraphicsInfo.GetModel("Sky"));
	sceneActors["Sky"] = skyActor;
	*/
	
	/*
	SceneActor* planetActor = new SceneActor();
	planetActor->SetModel(sceneGraphicsInfo.GetModel("Planet"));
	sceneActors["Planet1"] = planetActor;
	*/
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

	SceneActor* playerShip = GetActor("Player");
	if (playerShip != nullptr)
	{
		playerShip->SetPosition(vec3(0.0f, 0.0f, 0.0f));
		playerShip->SetScale(vec3(2.0f));
		playerShip->RotateAroundRight(glm::radians(90.0f));
		playerShip->RotateAroundUp(glm::radians(180.0f));
		playerShip->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	}

	SceneActor* rockActor;
	for (int i = 0; i < 5; i++)
	{
		rockActor = GetActor("TempRock" + to_string(i));
		if (rockActor != nullptr)
		{
			rockActor->SetScale(vec3(1.5f));
			rockActor->SetPosition(vec3((rand() % 132 - 32.0f), (rand() % 100), 0.0f));
			//rockActor->RotateAroundRight(glm::radians(90.0f));
			//rockActor->RotateAroundRight(glm::radians((float)(rand() % 180)));
		}
	}

	SceneActor* fighterActor = GetActor("Fighter1");
	if (fighterActor != nullptr)
	{
		fighterActor->SetScale(vec3(0.002f));
		fighterActor->RotateAroundUp(glm::radians(90.0f));
		fighterActor->RotateAroundRight(glm::radians(90.0f));
		fighterActor->RotateAroundForward(glm::radians(-90.0f));
		fighterActor->SetPosition(vec3(0.0f, 26.0f, 0.0f));
	}


	for (int i = 0; i < 5; i++)
	{
		fighterActor = GetActor("TempFighter" + to_string(i));
		if (fighterActor != nullptr)
		{
			fighterActor->SetScale(vec3(0.002f));
			fighterActor->RotateAroundUp(glm::radians(90.0f));
			fighterActor->RotateAroundRight(glm::radians(90.0f));
			fighterActor->RotateAroundForward(glm::radians(-90.0f));
			fighterActor->SetPosition(vec3((rand() % 132 - 32.0f), (rand() % 100), 0.0f));
		}
	}


	SceneActor* saucerActor = GetActor("Saucer1");
	if (saucerActor != nullptr)
	{
		saucerActor->SetScale(vec3(2.0f));
		saucerActor->RotateAroundRight(glm::radians(90.0f));
		saucerActor->SetPosition(vec3(2.0f, 89.0f, 0.0f));
	}
	/*
	SceneActor* skyActor = GetActor("Sky");
	if (skyActor != nullptr)
	{
		skyActor->SetScale(vec3(10.0f));
		skyActor->RotateAroundRight(glm::radians(90.0f));
		skyActor->SetPosition(vec3(0.0f, 0.0f, -20.0f));
	}
	*/
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
		cout << "Current Object = " << it->first << "\n";
		camera->SetPosition(it->second->Position() + vec3(0, 0, 20));
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
				if (it == sceneActors.end())
					it = sceneActors.begin();
			}
			break;
		case GLFW_KEY_UP:
			if (it != sceneActors.end())
			{
				it->second->Move(camera->Up());
			}
			break;
		case GLFW_KEY_RIGHT:
			if (it != sceneActors.end())
			{
				it->second->Move(camera->Right());
			}
			break;
		case GLFW_KEY_LEFT:
			if (it != sceneActors.end())
			{
				it->second->Move(camera->Left());
			}
			break;
		case GLFW_KEY_DOWN:
			if (it != sceneActors.end())
			{
				it->second->Move(camera->Down());
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

map<string, SceneActor*>* Scene::GetSceneActors()
{
	return &sceneActors;
}
