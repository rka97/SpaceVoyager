#include "Scene.h"
#include <iostream>
#include <string>
#include <time.h>
#include <glm/gtx/norm.hpp>
using namespace std;

BulletsController* formation;

long long lastUpdateTime = 0;
float theta = 0;
float dTheta = 0.1;

void Scene::LoadActors()
{
	lastUpdateTime = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
	formation = new BulletsController();
	sceneActors["Formation"] = formation;
	SceneActor* sa = new SceneActor();
	sa->SetModel(sceneGraphicsInfo.GetModel("Saucer"));
	sceneActors["Saucer"] = sa;

	vector<Bullet*> bullets;
	for (int i = 0; i < 500; i++) {
		Bullet* bullet = new Bullet();
		bullet->SetModel(sceneGraphicsInfo.GetModel("Bullet"));
		bullets.push_back(bullet);
	}
	formation->AddBullets(bullets);
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
	for (std::map<string, Drawable*>::iterator it = sceneActors.begin(); it != sceneActors.end(); it++)
	{
		if (it->second != nullptr)
			delete it->second;
	}
}

void Scene::setEditorMode(bool editorMode)
{
	this->editorMode = editorMode;
}

glm::vec3 Spiral(float& t, glm::vec3& center, glm::vec3& rightVector, glm::vec3& upVector) {
	return center + (cos(t) * rightVector + sin(t) * upVector) * t * t;
}

glm::vec3 Curvilinear(float dT, glm::vec3& velocity, glm::vec3& acceleration, glm::vec3& jerk, float& speed) {
	float velocityNorm = l2Norm(velocity);
	vec3 tangent = velocity * (1 / velocityNorm);
	vec3 normal = vec3(tangent.y, -tangent.x, 0);
	acceleration += dT * jerk;
	velocity += (tangent * acceleration.x + normal * acceleration.y) * dT;
	if(speed != -1)
		return dT * (velocity * speed / velocityNorm);
	return dT * velocity;
}

void Scene::UpdateSceneGameMode()
{
	formation->Update();

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
		case GLFW_KEY_X: 
		{
			float tmpTheta = theta;
			for (int i = 0; i < 10; i++) {
				vec3 pos = vec3(cos(tmpTheta), sin(tmpTheta), 0) * 10.0f;
				formation->ActivateBullet(Curvilinear, pos, vec3(pos) * 3.0f, vec3(20, 19, 0), vec3(0), 10);
				tmpTheta += 2 * M_PI / 10;
			}
			theta += dTheta;
			break;
		}
		case GLFW_KEY_Z:
			break;
		case GLFW_KEY_C:
			break;
		default:
			break;
		}
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

Drawable * Scene::GetActor(string name)
{
	if (sceneActors.find(name) == sceneActors.end())
	{
		cout << "Error in Scene::GetActor: could not find actor with name " << name << "!\n";
		return nullptr;
	}
	return sceneActors[name];
}

map<string, Drawable*>* Scene::GetSceneActors()
{
	return &sceneActors;
}
