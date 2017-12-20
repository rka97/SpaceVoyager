#include "Enemy.h"


Enemy::Enemy(SceneActor* sceneActor, BulletsController* bulletsController)
{
	this->sceneActor = sceneActor;
	this->bulletsController = bulletsController;
}


Enemy::~Enemy()
{
}

glm::vec3 Enemy::GetPosition()
{
	return sceneActor->Position();
}

int Enemy::GetMaxHealth()
{
	return maximumHealth;
}
