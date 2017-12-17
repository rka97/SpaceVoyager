#include "Enemy.h"


Enemy::Enemy(SceneActor* sceneActor, BulletsController* bulletsController)
{
	this->sceneActor = sceneActor;
	this->bulletsController = bulletsController;
}


Enemy::~Enemy()
{
}
