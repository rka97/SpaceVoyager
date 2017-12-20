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
SceneActor* skyActor;

int playerShootTime = -1;
int playerShootPeriod = 300;

float time_now_f = 0;

void Scene::LoadActors()
{
	formation = new BulletsController();
	
	planetActor = new SceneActor();
	planetActor->SetModel(sceneGraphicsInfo.GetModel("Saucer"));
	planetActor->RotateAroundRight(glm::radians(90.0f));

	playerShip = new SceneActor();
	playerShip->SetModel(sceneGraphicsInfo.GetModel("Imperial"));

	playerShip->SetPosition(vec3(0.0f, -150.0f, -100.f));
	playerShip->SetScale(vec3(1.25f));
	playerShip->RotateAroundRight(glm::radians(90.0f));

	vector<Bullet*> bullets;
	for (int i = 0; i < 1500; i++) {
		Bullet* bullet = new Bullet();
		bullet->SetModel(sceneGraphicsInfo.GetModel("Bullet"));
		bullets.push_back(bullet);
		bullet->SetSize(glm::vec2(3,3));
	}
	formation->AddBullets(bullets);
	planetBoss = new PlanetBoss(planetActor, formation);

	skyActor = new SceneActor();
	skyActor->SetModel(sceneGraphicsInfo.GetModel("Sky"));
	skyActor->SetPosition(vec3(0.0f, 0.0f, -180.0f));
	skyActor->RotateAroundRight(glm::radians(90.0f));
	skyActor->SetScale(vec3(15.0f));

	sceneActors.push_back(planetActor);
	sceneActors.push_back(playerShip);
	sceneActors.push_back(formation);
	sceneActors.push_back(skyActor);
}


void Scene::Initialize()
{
	camera = new Camera();
	mainLight = new Light(true);
	sceneGraphicsInfo.Initialize();
	LoadActors();

	/* put the camera at the positive z-axis */;
	camera->SetPosition(glm::vec3(0, 0, 40));
	/* turn the camera back to the origin */
	camera->RotateAroundUp(glm::radians(180.0f));
	//camera->ZoomOut(glm::radians(50.0f));
	camera->SetClippingPlanes(1, 250);
	camera->ZoomOut(glm::radians(10.0f));

	mainLight->SetPosition(glm::vec3(0, 0, -1.0f));
	playerShootTime = timeNow;
}


Scene::Scene()
{
	camera = nullptr;
	mainLight = nullptr;
	inputBuffer = nullptr;
	keyStates[0] = 0;
	keyStates[1] = 0;
	keyStates[2] = 0;
	keyStates[3] = 0;
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

void Scene::UpdateKeyboardInput()
{
	while (!inputBuffer->empty()) {
		KeyboardEvent e = inputBuffer->front();
		inputBuffer->pop();
		switch (e.code)
		{
		case GLFW_KEY_W:
			if (e.eventType == GLFW_PRESS)
				keyStates[0] = 1;
			else if (e.eventType == GLFW_RELEASE)
				keyStates[0] = 0;
			break;
		case GLFW_KEY_A:
			if (e.eventType == GLFW_PRESS)
				keyStates[1] = 1;
			else if (e.eventType == GLFW_RELEASE)
				keyStates[1] = 0;
			break;
		case GLFW_KEY_S:
			if (e.eventType == GLFW_PRESS)
				keyStates[2] = 1;
			else if (e.eventType == GLFW_RELEASE)
				keyStates[2] = 0;
			break;
		case GLFW_KEY_D:
			if (e.eventType == GLFW_PRESS)
				keyStates[3] = 1;
			else if (e.eventType == GLFW_RELEASE)
				keyStates[3] = 0;
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
	if (keyStates[0])
	{
		playerShip->Move(vec3(0, 1.6, 0));
	}
	if (keyStates[1])
	{
		playerShip->Move(vec3(-1.6, 0, 0));
	}
	if (keyStates[2])
	{
		playerShip->Move(vec3(0, -1.6, 0));
	}
	if (keyStates[3])
	{
		playerShip->Move(vec3(1.6, 0, 0));
	}
}

float NormVec2(glm::vec2& vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

int rotAroundZ = 0;

void Scene::UpdateMouseInput()
{
	if (mouseListener->changed)
	{
		vec3 playerPos = playerShip->Position();
		vec2 mousePos = mouseListener->position;
		//x [-245, 245] <- [0, 1024]
		//y [-163,163] <- [0, 684]
		vec2 newPos = vec2((mousePos.x / 1024.0f) *2*245 - 245, ((684.0f - mousePos.y) / 684.0f) * 2*163 - 163);
		cout << "P: (" << playerPos.x << ", " << playerPos.y << ", " << playerPos.z << "). \n";
		cout << "M: (" << mousePos.x << ", " << mousePos.y << "). \n";
		vec3 newPlayerPos = vec3(newPos.x, newPos.y, playerPos.z);
		playerShip->SetPosition(newPlayerPos);
		/* 
		vec2 positionChange = mouseListener->position - mouseListener->prevPosition;
		vec2 playerPos2D = vec2((playerPos.x + 135) * 1024.0f / (2 * 135), 684.0f - ((playerPos.y + 140) * 684.0f / (2 * 140)));
		vec2 playerPos2D = vec2((playerPos.x + 135) * 1024.0f / (2 * 135), 684.0f - ((playerPos.y + 140) * 684.0f / (2*140)));
		vec2 playerDirection = playerShip->Forward();
		vec2 oldRay = playerDirection / NormVec2(playerDirection);
		vec2 newRay = mouseListener->position - playerPos2D;
		float norm = NormVec2(newRay);
		if (norm < 0.001f)
			norm = 0.001f;
		newRay /= norm;
		float angle = atan2f(newRay.y, newRay.x) - atan2f(oldRay.y, oldRay.x);
		playerShip->Rotate(vec3(0.0f, 1.0f, 0.0f), -1 * angle * 0.05f);
		mouseListener->changed = false;
		*/
	}
}


void Scene::UpdateScene()
{
	formation->Update();
	planetBoss->Update(vec3(-3,-3,-100));
	float currentPlayerSpeed = 1;
	float numBullets = l2Norm(planetBoss->GetPosition() - playerShip->Position()) * -15.0f / 300 + 15.0f;

	if (timeNow - playerShootTime >= playerShootPeriod) {
		vec3 bulletStartingPosition = playerShip->GetTransformationMatrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
		formation->PlayerAttack(bulletStartingPosition, numBullets, radians(30.0f));
		playerShootTime = timeNow;
	}
	if (false /* playerShip->ShouldDie == true */)
	{
		/* Player death explosion */
		formation->PlayerExplosion(playerShip->Position(), 40, radians(360.0f));
		formation->PlayerExplosion(playerShip->Position(), 20, radians(360.0f));
	}
	time_now_f = (float)timeNow/1000;
	skyActor->SetParameterValue(skyActor->GetParameterID("iTime"), &time_now_f);
	Rectangle2D rect = planetActor->GetEnclosingRectangle();
	UpdateKeyboardInput();
	UpdateMouseInput();
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

bool Scene::SetMouseListener(MouseChange * chg)
{
	if (chg == nullptr)
		return false;
	this->mouseListener = chg;
	return true;
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
