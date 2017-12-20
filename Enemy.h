#pragma once
#include "SceneActor.h"
#include "BulletsController.h"

class Enemy
{
public:
	Enemy(SceneActor* sceneActor, BulletsController* bulletsController);
	~Enemy();
	void SetSceneActor(SceneActor* sceneActor);
	glm::vec3 GetPosition();
	virtual void Update(glm::vec3 pos) = 0;

protected:
	int maximumHealth;
	int currentHealth;
	SceneActor* sceneActor;
	BulletsController* bulletsController;
};

