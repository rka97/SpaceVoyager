#pragma once
#include "Enemy.h"
class PlanetBoss : public Enemy
{

public:
	PlanetBoss(SceneActor* sceneActor, BulletsController* bulletsController);
	void Update(vec3 pos);
	void inModeUpdate();
	void SetCrazyMode();
	~PlanetBoss();
private:
	void Reset();
	enum Mode{
		NORMAL_MODE, MOVE1_MODE, CRAZY_MODE, MOVE2_MODE, WAIT_MODE
	};
	Mode mode;
	Mode nxtMode;
	int waitPeriod;
	glm::vec3 initialPosition;
	glm::vec3 modePosition;
	float theta;
	float dTheta;
	long long spiralShootTime;
	long long directedShootTime;
	long long movementTime;
	long long waitTime;
	long long crazyTime;
	int spiralShootPeriod;
	int crazySpiralShootPeriod;
	int crazyPeriod;
	int directedShootPeriod;
	bool inMode;
	bool moveMode;
	int crazyNumberOfBullets;
	int numBulletsShot;
	bool flag = false;
	
};

