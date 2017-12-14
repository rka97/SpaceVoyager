#include "BulletsController.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <iostream>
using std::cout;

int pos_loc = 0;
int color_loc = 1;
int view_loc = 0;
int proj_loc = 1;
int center_loc = 2;
int size_loc = 3;
int angle_loc = 4;

float clamp(float val, float min, float max) {
	if (val > max) return max;
	if (val < min) return min;
	return val;
}

BulletsController::BulletsController() {
	lastUpdateTime = timeNow;
	bulletPositions = new vec3[MAX_NUM_BULLETS];
}

BulletsController::~BulletsController() 
{
	delete[] bulletPositions;
}

void BulletsController::Update() {
	int realdT = timeNow - lastUpdateTime;
	float dT = realdT / 500.0f;
	for (int i = 0; i < liveBullets.size(); i++) {
		BulletInfo* bi = liveBullets[i];
		switch (bi->type) {
		case DIFFERENTIAL: {
			Differential* dif = (Differential*)bi->formationInfo;
			bi->bullet->SetPosition(bi->bullet->Position() + dif->update(dT, dif->velocity, dif->acceleration, dif->jerk, dif->speed));
			bi->lifeTime -= realdT;
			break;
		}
		case PARAMETRIC:		
			Parametric* par = (Parametric*)bi->formationInfo;
			par->t += dT;
			vec3 pos = par->update(par->t, par->center, par->rightVector, par->upVector);
			bi->bullet->SetPosition(pos);
			bi->lifeTime -= realdT;
			break;
		}
		if (bi->lifeTime <= 0) {
			delete bi->formationInfo;
			bi->formationInfo = nullptr;
			bi->lifeTime = standardLifeTime;
			BulletInfo* bi2 = liveBullets.back();
			liveBullets[i] = bi2;
			admittedBullets.push_back(bi);
			liveBullets.pop_back();
			i--;
		}

	}
	lastUpdateTime = timeNow;
}

void BulletsController::Draw(SceneInfo& sceneInfo, int stupid)
{
	if (liveBullets.empty())
	{
		return;
	}
	
	int i = 0;
	for (auto& bulletInfo : liveBullets) {
		bulletPositions[i].x = bulletInfo->bullet->Position().x;
		bulletPositions[i].y = bulletInfo->bullet->Position().y;
		bulletPositions[i].z = bulletInfo->bullet->Position().z;

		i++;
	}
	liveBullets[0]->bullet->GetModel()->InitializeInstanced(this->bulletPositions, MAX_NUM_BULLETS);
	liveBullets[0]->bullet->Draw(sceneInfo, liveBullets.size());
}

bool BulletsController::ActivateBullet(const std::function<vec3(float&, vec3&, vec3&, vec3&)>& func, float initialT, vec3 center, vec3 rightVector, vec3 upVector) {
	if (admittedBullets.empty()) return false;
	BulletInfo* bi = admittedBullets.back();
	admittedBullets.pop_back();
	liveBullets.push_back(bi);

	Parametric* par = new Parametric;
	par->t = initialT;
	par->update = func;
	par->center = center;
	par->rightVector = rightVector;
	par->upVector = upVector;
	bi->formationInfo = par;
	bi->type = PARAMETRIC;
	return true;
}

bool BulletsController::ActivateBullet(std::function<vec3(float, vec3&, vec3&, vec3&, float&)> func, vec3 initialPos, vec3 velocity, vec3 acceleration, vec3 jerk, float speed) {
	if (admittedBullets.empty()) return false;
	BulletInfo* bi = admittedBullets.back();
	admittedBullets.pop_back();
	liveBullets.push_back(bi);
	bi->bullet->SetPosition(initialPos);
	Differential* dif = new Differential;
	dif->acceleration = acceleration;
	dif->velocity = velocity;
	bi->type = DIFFERENTIAL;
	bi->formationInfo = dif;
	dif->jerk = jerk;
	dif->speed = speed;
	dif->update = func;
}

void BulletsController::SetCenter(glm::vec3 center) {
	this->center = center;
}

void BulletsController::AddBullets(vector<Bullet*> bullets) {
	for (auto& bullet : bullets) {
		BulletInfo* bi = new BulletInfo;
		bi->bullet = bullet;
		admittedBullets.push_back(bi);
	}
	
}
