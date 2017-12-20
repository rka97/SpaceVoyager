#include "Scene.h"
#include <iostream>
#include <string>
#include <time.h>
#include "PlanetBoss.h"
#include "SpaceShip.h"
using namespace std;

BulletsController* formation;

float theta = 0;
float dTheta = 0.1;

SceneActor* planetActor;
PlanetBoss* planetBoss;
SpaceShip * playerShip;
SceneActor* skyActor;
SceneActor* healthBarActor;
SceneActor* shieldBarActor;
SceneActor* StartScreen;
SceneActor* EndScreen;
int timeID;
int healthID;
int maxHealthID;
int health;
int maxHealth;
int shield;
int maxShield;
int playerShootTime = -1;
int playerShootPeriod = 300;

float time_now_f = 0;

void Scene::IntialLoad()
{
	StartScreen = new SceneActor();
	StartScreen->SetModel(sceneGraphicsInfo.GetModel("Start"));
	StartScreen->SetPosition(vec3(0.0, 0.0, -10.0));
	StartScreen->SetScale(vec3(50.0f));
	StartScreen->RotateAroundRight(glm::radians(90.0f));
	sceneActors.push_back(StartScreen);
}


void Scene::LoadActors()
{
	sceneActors.pop_back();
	formation = new BulletsController();
	
	planetActor = new SceneActor();
	planetActor->SetModel(sceneGraphicsInfo.GetModel("Saucer"));
	planetActor->RotateAroundRight(glm::radians(90.0f));

	vector<Bullet*> bullets;
	for (int i = 0; i < 3000; i++) {
		Bullet* bullet = new Bullet();
		bullet->SetModel(sceneGraphicsInfo.GetModel("Bullet"));
		bullets.push_back(bullet);
		bullet->SetSize(glm::vec2(3,3));
	}
	formation->AddBullets(bullets);

	playerShip = new SpaceShip();
	playerShip->SetModel(sceneGraphicsInfo.GetModel("Imperial"));

	playerShip->SetPosition(vec3(0.0f, -150.0f, -100.f));
	playerShip->SetScale(vec3(1.25f));
	playerShip->RotateAroundRight(glm::radians(90.0f));
	playerShip->SetHeartModel(sceneGraphicsInfo.GetModel("Bullet"));
	BulletInfo* bt = new BulletInfo;
	bt->bullet = playerShip->GetHeart();
	vector<BulletInfo*> shipHeartBulletsInfo;
	shipHeartBulletsInfo.push_back(bt);
	formation->AddPattern(shipHeartBulletsInfo, 1);

	planetBoss = new PlanetBoss(planetActor, formation);

	healthBarActor = new SceneActor();
	healthBarActor->SetModel(sceneGraphicsInfo.GetModel("HealthBar"));
	healthBarActor->RotateAroundRight(glm::radians(90.0f));
	healthBarActor->SetScale(vec3(15.0f, 0.2f, 0.05f));
	healthBarActor->SetPosition(vec3(0.0f, 188, -120.0f));

	shieldBarActor = new SceneActor();
	shieldBarActor->SetModel(sceneGraphicsInfo.GetModel("ShieldBar"));
	shieldBarActor->RotateAroundRight(glm::radians(90.0f));
	shieldBarActor->SetScale(vec3(15.0f, 0.2f, 0.05f));
	shieldBarActor->SetPosition(vec3(0.0f, -188, -120.0f));

	skyActor = new SceneActor();
	skyActor->SetModel(sceneGraphicsInfo.GetModel("Sky"));
	skyActor->SetPosition(vec3(0.0f, 0.0f, -180.0f));
	skyActor->RotateAroundRight(glm::radians(90.0f));
	skyActor->SetScale(vec3(15.0f));

	sceneActors.push_back(planetActor);
	sceneActors.push_back(playerShip);
	sceneActors.push_back(formation);
	sceneActors.push_back(skyActor);
	sceneActors.push_back(healthBarActor);
	sceneActors.push_back(shieldBarActor);

	timeID = skyActor->GetParameterID("iTime");
	healthID = healthBarActor->GetParameterID("health");
	maxHealthID = healthBarActor->GetParameterID("maxHealth");
	maxHealth = planetBoss->GetMaxHealth();
	healthBarActor->SetParameterValue(maxHealthID, &maxHealth);
	shieldBarActor->SetParameterValue(maxHealthID, &playerShip->maxTicksToShield);
}

void Scene::FinalLoad()
{
	sceneActors.pop_back();
	sceneActors.pop_back();
	sceneActors.pop_back();
	sceneActors.pop_back();

	EndScreen = new SceneActor();
	EndScreen->SetModel(sceneGraphicsInfo.GetModel("GameOver"));
	EndScreen->SetScale(vec3(20.0f));
	EndScreen->RotateAroundRight(glm::radians(90.0f));
	sceneActors.push_back(EndScreen);
}

void Scene::Initialize()
{
	camera = new Camera();
	mainLight = new Light(true);
	sceneGraphicsInfo.Initialize();
	IntialLoad();
	//LoadActors();

	/* put the camera at the positive z-axis */;
	camera->SetPosition(glm::vec3(0, 0, 40));
	/* turn the camera back to the origin */
	camera->RotateAroundUp(glm::radians(180.0f));
	camera->SetClippingPlanes(1, 250);
	camera->ZoomOut(glm::radians(10.0f));

	mainLight->SetPosition(glm::vec3(0, 0, -1.0f));
	playerShootTime = timeNow;
}
void Scene::PlayerDeathScene()
{
	formation->PlayerExplosion(playerShip->Position(), 400, radians(360.0f));
	playerShip->SetScale(vec3(0));
	playerShip->GetHeart()->SetSize(vec2(0));
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
		case GLFW_KEY_Q:
			if (playerShip->ticksToShield >= playerShip->maxTicksToShield)
			{
				formation->Shield(playerShip->Position(), 1, glm::radians(0.0f));
				playerShip->ticksToShield = 0;
			}
			break;
		}
	}
	/*
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
	}*/
}

void Scene::UpdateMouseInput()
{
	if (mouseListener->changed)
	{
		vec3 playerPos = playerShip->Position();
		vec2 mousePos = mouseListener->position;
		// x [-245, 245] <- [0, 1024], y [-163,163] <- [0, 684]
		vec2 newPos = vec2((mousePos.x / 1024.0f) *2*245 - 245, ((684.0f - mousePos.y) / 684.0f) * 2*163 - 163);
		vec3 newPlayerPos = vec3(newPos.x, newPos.y, playerPos.z);
		playerShip->SetPosition(newPlayerPos);
	}
}

bool Scene::UpdatePhysics()
{
	if (playerShip->dead == true || planetBoss->GetHealth() <= 0)
		return false;

	float numBullets = l2Norm(planetBoss->GetPosition() - playerShip->Position()) * -15.0f / 300 + 15.0f;
	if (timeNow - playerShootTime >= playerShootPeriod) {
		vec3 bulletStartingPosition = playerShip->GetTransformationMatrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
		formation->PlayerAttack(bulletStartingPosition, numBullets, radians(30.0f));
		playerShootTime = timeNow;
	}
	vector<BulletInfo*> enemyLiveBullets = formation->GetLiveBullets(0);
	for (auto& enemyBullet : enemyLiveBullets)
	{
		if (playerShip->CheckCollision(enemyBullet->bullet))
		{
			enemyBullet->lifeTime = -1;
			playerShip->dead = true;
			PlayerDeathScene();
			return false;
		}

	}
	vector<BulletInfo*> shipLiveBullets = formation->GetLiveBullets(1);
	for (auto& playerBullet : shipLiveBullets)
	{
		if (planetActor->CheckCollision(playerBullet->bullet))
		{
			playerBullet->lifeTime = 10;
			formation->Sharpnel(playerBullet->bullet->Position(), 8, glm::radians(360.0f));

			if (!planetBoss->Decrementhealth())
			{
				formation->BossExplosion(planetBoss->GetPosition(), 200, glm::radians(360.0f));
				planetBoss->sceneActor->SetScale(vec3(0.0f));
				return false;
			}
		}

	}
	vector<BulletInfo*> activeShields = formation->GetLiveBullets(4);
	for (auto& shield : activeShields)
	{
		for (auto& enemyBullet : enemyLiveBullets)
		{
			if (shield->bullet->CheckCollision(enemyBullet->bullet))
			{
				enemyBullet->lifeTime = -1;
			}
		}
		if (planetActor->CheckCollision(shield->bullet))
		{
			shield->lifeTime = 10;
			formation->Sharpnel(shield->bullet->Position(), 16, glm::radians(360.0f));

			if (!planetBoss->Decrementhealth(10))
			{
				formation->BossExplosion(planetBoss->GetPosition(), 200, glm::radians(360.0f));
				planetBoss->sceneActor->SetScale(vec3(0.0f));
				return false;
			}
		}
	}
	if (planetActor->CheckCollision(playerShip))
	{
		playerShip->dead = true;
		PlayerDeathScene();
	}
	return true;
}

int ticks = 0;
bool Scene::UpdateScene()
{
	float currentPlayerSpeed = 1;
	time_now_f = (float)timeNow / 1000;
	skyActor->SetParameterValue(timeID, &time_now_f);
	health = planetBoss->GetHealth();
	healthBarActor->SetParameterValue(healthID, &health);
	shieldBarActor->SetParameterValue(healthID, &playerShip->ticksToShield);
	
	UpdateKeyboardInput();
	UpdateMouseInput();
	formation->Update();
	if (health > 0)
	{
		planetBoss->Update(vec3(-3, -3, -100));
		ticks++;
		if (ticks >= 1024)
		{
			planetBoss->SetCrazyMode();
			ticks = 0;
		}
	}
	if (!playerShip->dead)
	{
		playerShip->ticksToShield++;
		if (playerShip->ticksToShield >= playerShip->maxTicksToShield)
			playerShip->ticksToShield = playerShip->maxTicksToShield;
	}
	return UpdatePhysics();
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
