#include "Scene.h"
#include <iostream>
#include <string>
#include <time.h>
#include "PlanetBoss.h"
using namespace std;

BulletsController* formation;

float theta = 0;
float dTheta = 0.1;

SceneActor* planetActor;
PlanetBoss* planetBoss;
SceneActor* playerShip;

int playerShootTime = -1;
int playerShootPeriod = 300;

void Scene::LoadActors()
{
	formation = new BulletsController();
	
	planetActor = new SceneActor();
	planetActor->SetModel(sceneGraphicsInfo.GetModel("Saucer"));
	playerShip = new SceneActor();
	playerShip->SetModel(sceneGraphicsInfo.GetModel("Imperial"));

	playerShip->SetPosition(vec3(0.0f, 0.0f, -100.f));
	playerShip->SetScale(vec3(8.0f));
	playerShip->RotateAroundRight(glm::radians(90.0f));
	playerShip->RotateAroundUp(glm::radians(180.0f));

	planetActor->RotateAroundRight(glm::radians(90.0f));

	vector<Bullet*> bullets;
	for (int i = 0; i < 1500; i++) {
		Bullet* bullet = new Bullet();
		bullet->SetModel(sceneGraphicsInfo.GetModel("Bullet"));
		bullets.push_back(bullet);
		bullet->SetSize(glm::vec2(3,3));
	}
	formation->AddBullets(bullets);

	
	planetBoss = new PlanetBoss(planetActor, formation);
	
	sceneActors.push_back(planetActor);
	sceneActors.push_back(playerShip);
	sceneActors.push_back(formation);
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
	//camera->ZoomOut(glm::radians(50.0f));
	camera->SetClippingPlanes(1, 150);
	camera->ZoomOut(180);

	mainLight->SetPosition(glm::vec3(0, 0, -1.0f));
	playerShootTime = timeNow;
}


Scene::Scene()
{
	camera = nullptr;
	mainLight = nullptr;
	inputBuffer = nullptr;
}

Scene::~Scene()
{
	if (camera != nullptr)
		delete camera;
	if (mainLight != nullptr)
		delete mainLight;
	for (std::vector<Drawable*>::iterator it = sceneActors.begin(); it != sceneActors.end(); it++)
	{
		if (*it != nullptr)
			delete *it;
	}
}

void Scene::UpdateScene()
{
	formation->Update();
	planetBoss->Update(vec3(-3,-3,-100));
	if (timeNow - playerShootTime >= playerShootPeriod) {
		formation->Explosion(playerShip->GetTransformationMatrix() * glm::vec4(0, 2.5f, 0.0f, 1.0f), 40, 0.3);
		// formation->Explosion(playerShip->GetTransformationMatrix() * glm::vec4(0, 2.5f, 0.0f, 1.0f), 40, 0.3);
		playerShootTime = timeNow;
	}


	Rectangle2D rect = planetActor->GetEnclosingRectangle();

	while (!inputBuffer->empty()) {
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
			playerShip->Move(vec3(0,1.6,0));
			break;
		case GLFW_KEY_A:
			playerShip->Move(vec3(-1.6, 0, 0));
			break;
		case GLFW_KEY_D:
			playerShip->Move(vec3(1.6, 0, 0));
			break;
		case GLFW_KEY_S:
			playerShip->Move(vec3(0, -1.6, 0));
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
		case GLFW_KEY_X:
			// formation->ActivateShield(playerShip->Position(), vec3(0), vec3(0), 20, 0);
			break;
		case GLFW_KEY_KP_ADD:
			camera->ZoomIn(glm::radians(1.0f));
			break;
		case GLFW_KEY_KP_SUBTRACT:
			camera->ZoomOut(glm::radians(1.0f));
			break;
		case GLFW_KEY_Y:
			planetBoss->SetCrazyMode();
			break;
		default:
			break;
		}
	}		
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

vector<Drawable*> Scene::GetSceneActors()
{
	return sceneActors;
}
