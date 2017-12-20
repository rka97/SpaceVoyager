#include "PlanetBoss.h"
#define planet_pattern_id 0

PlanetBoss::PlanetBoss(SceneActor* sceneActor, BulletsController* bulletsController) : Enemy(sceneActor, bulletsController)
{
	theta = 0;
	dTheta = 0.1f;
	movementTime = -1;
	spiralShootPeriod = 1500;
	crazySpiralShootPeriod = 100;
	directedShootPeriod = 300;
	lastTime = timeNow;
	initialPosition = vec3(0, 50, -100);
	modePosition = vec3(0, -100, -100);
	sceneActor->SetPosition(initialPosition);
	sceneActor->SetScale(vec3(10.f));
	inMode = true;
	moveMode = true;
	crazyNumberOfBullets = 500;
	mode = NORMAL_MODE;
	numBulletsShot = 0;
	maximumHealth = 30000;
	health = maximumHealth;
	flag = true;
}

void PlanetBoss::Update(vec3 pos) {
	if (movementTime == -1)	Reset();
	auto rotationSpeed = (timeNow - lastTime) * 0.05f;
	lastTime = timeNow;
	sceneActor->RotateAroundUp(glm::radians(rotationSpeed));
	switch (mode)
	{
	case NORMAL_MODE:
	{
		int dTSpiral = timeNow - spiralShootTime;
		float tmpTheta = theta;
		vec3 planetPos = sceneActor->Position();
		if (dTSpiral >= spiralShootPeriod) {
			for (int i = 0; i < 40; i++) {
				glm::vec3 pos = vec3(cos(tmpTheta), sin(tmpTheta), 0);
				bulletsController->ActivateBullet(planet_pattern_id, Curvilinear, planetPos, pos * 10.0f, vec3(2), vec3(0, 0, 0), 30);
				tmpTheta += 2 * M_PI / 40;
			}
			theta += 0.1;
			spiralShootTime = timeNow;
		}

		int dTDirected = timeNow - directedShootTime;
		if (dTDirected >= directedShootPeriod) {
			float randTheta = rand() % 1000;
			float t = glm::radians((float)(rand() % 360));
			vec3 dir = vec3(cos(t), sin(t), 0);

			int initialSpeed = 40;
			for (int i = 0; i < 10; i++) {
				bulletsController->ActivateBullet(planet_pattern_id, Curvilinear, sceneActor->Position() + dir, dir, vec3(0), vec3(0), initialSpeed + i);
			}
			directedShootTime = timeNow;
		}
		if (currentHealth <= 0.5 * maximumHealth && !flag) {
			mode = WAIT_MODE;
			waitPeriod = 2000;
			nxtMode = MOVE1_MODE;
			flag = true;
			Reset();
		}
		break;
	}
	case MOVE1_MODE:
	{
		int dT = timeNow - movementTime;
		vec3 u = modePosition - sceneActor->Position();
		sceneActor->SetPosition(sceneActor->Position() + normalize(u) * (0.1f * dT));
		movementTime = timeNow;
		if (l2Norm(sceneActor->Position() - modePosition) < 0.1) {
			mode = WAIT_MODE;
			waitPeriod = 2000;
			nxtMode = CRAZY_MODE;
			Reset();
		}
		break;
	}
	case CRAZY_MODE:
	{
		sceneActor->RotateAroundUp(glm::radians(10.0f));
		int dTSpiral = timeNow - spiralShootTime;
		float tmpTheta = theta;
		vec3 planetPos = sceneActor->Position();
		if (dTSpiral >= crazySpiralShootPeriod) {
			for (int i = 0; i < 20; i++) {
				glm::vec3 pos = vec3(cos(tmpTheta), sin(tmpTheta), 0);
				bulletsController->ActivateBullet(planet_pattern_id, Curvilinear, planetPos, pos * 10.0f, vec3(2), vec3(0, 0, 0), 40);
				tmpTheta += 2 * M_PI / 20;
			}
			theta += 0.1;
			spiralShootTime = timeNow;
			numBulletsShot += 20;
			if (numBulletsShot >= 1000) {
				movementTime = -1;
				mode = WAIT_MODE;
				waitPeriod = 2000;
				nxtMode = MOVE2_MODE;
				Reset();
			}
		}
		break;
	}
	case WAIT_MODE:
	{
		if (timeNow - waitTime >= waitPeriod) {
			mode = nxtMode;
			Reset();
		}
		break;
	}
	case MOVE2_MODE:
	{
		if (movementTime == -1) { movementTime = timeNow; return; }
		int dT = timeNow - movementTime;
		if (moveMode) {
			vec3 u = initialPosition - sceneActor->Position();
			sceneActor->SetPosition(sceneActor->Position() + normalize(u) * (0.1f * dT));
			movementTime = timeNow;
			if (l2Norm(u) < 0.1) {
				mode = WAIT_MODE;
				waitPeriod = 2000;
				nxtMode = NORMAL_MODE;
				Reset();
				mode = NORMAL_MODE;
			}
			return;
		}
		break;
	}
	}
}

void PlanetBoss::Reset() {
	spiralShootTime = timeNow;
	directedShootTime = timeNow;
	movementTime = timeNow;
	waitTime = timeNow;
	crazyTime = timeNow;
	numBulletsShot = 0;
}

void PlanetBoss::SetCrazyMode() {
	this->mode = MOVE1_MODE;
	Reset();
}
bool PlanetBoss::Decrementhealth(int dec)
{
	health -= dec;
	if (health <= 0)
		return false;
	return true;
}

int PlanetBoss::GetHealth()
{
	return health;
}

void PlanetBoss::inModeUpdate() {}

PlanetBoss::~PlanetBoss() {}
