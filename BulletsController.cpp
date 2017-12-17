#include "BulletsController.h"
#include <glm/gtx/norm.hpp>

int pos_loc = 0;
int color_loc = 1;
int view_loc = 0;
int proj_loc = 1;
int center_loc = 2;
int size_loc = 3;
int angle_loc = 4;

BulletsController::BulletsController() {
	lastUpdateTime = timeNow;
	bulletPositions = new vec3[MAX_NUM_BULLETS];
	playerBulletPositions = new vec3[MAX_PLAYER_BULLETS];
	shield = nullptr;
}

BulletsController::~BulletsController() 
{
	delete[] bulletPositions;
}

void BulletsController::_Update(vector<BulletInfo*>& bullets, int realdT, float dT) {
	for (int i = 0; i < bullets.size(); i++) {
		BulletInfo* bi = bullets[i];
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
			BulletInfo* bi2 = bullets.back();
			bullets[i] = bi2;
			admittedBullets.push_back(bi);
			bullets.pop_back();
			i--;
		}
	}
}

void BulletsController::Update() {
	int realdT = timeNow - lastUpdateTime;
	float dT = realdT / 500.0f;
	
	// Update enemy bullets
	_Update(liveBullets, realdT, dT);

	// Update player bullets
	_Update(playerLiveBullets, realdT, dT);

	lastUpdateTime = timeNow;
}

void BulletsController::Draw(SceneInfo& sceneInfo, int stupid)
{
	// Draw enemey bullets
	if (!liveBullets.empty()) {
		int i = 0;
		for (auto& bulletInfo : liveBullets) {
			bulletPositions[i] = bulletInfo->bullet->Position();
			i++;
		}
		liveBullets[0]->bullet->GetModel()->InitializeInstanced(this->bulletPositions, MAX_NUM_BULLETS);
		liveBullets[0]->bullet->Draw(sceneInfo, liveBullets.size());
	}

	// Draw player bullets
	if (!playerLiveBullets.empty()){
		int i = 0;
		for (auto& bulletInfo : playerLiveBullets) {
			playerBulletPositions[i] = bulletInfo->bullet->Position();
			i++;
		}
		playerLiveBullets[0]->bullet->GetModel()->InitializeInstanced(this->playerBulletPositions, MAX_PLAYER_BULLETS);
		playerLiveBullets[0]->bullet->Draw(sceneInfo, playerLiveBullets.size());
	}
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

	Differential* dif = nullptr;
	if (bi->type != DIFFERENTIAL) {
		if (bi->formationInfo != nullptr) delete bi->formationInfo;
		dif = new Differential;
		bi->formationInfo = dif;
		bi->type = DIFFERENTIAL;
	}
	else {
		dif = (Differential*)bi->formationInfo;
	}

	dif->acceleration = acceleration;
	dif->velocity = velocity;
	dif->jerk = jerk;
	dif->speed = speed;
	dif->update = func;
	
	bi->bullet->SetMiddleColor(vec4(1, 0, 0, 1));
	bi->bullet->SetSize(vec2(3, 3));
	bi->lifeTime = standardLifeTime;
}

bool BulletsController::ActivateShield(vec3 pos, vec3 velocity, vec3 acceleration, float initialSize, float maximumSize)
{
	return false;
}

bool BulletsController::PlayerAttack(vec3 pos, int num, float angle)
{
	if (admittedBullets.size() < num) return false;
	float thetaBegin = (M_PI - angle) / 2;
	float deltaT = angle / (num - 1);
	
	for (int i = 0; i < num; i++) {
		BulletInfo* bi = admittedBullets.back();
		admittedBullets.pop_back();
		playerLiveBullets.push_back(bi);
		bi->bullet->SetPosition(pos);
		bi->bullet->SetMiddleColor(vec4(0, 0, 1, 0.8));
		bi->bullet->SetSize(vec3(3, 3, 3));
		
		Differential* dif = nullptr;
		if (bi->type != DIFFERENTIAL) {
			if (bi->formationInfo != nullptr) delete bi->formationInfo;
			dif = new Differential;
			bi->formationInfo = dif;
			bi->type = DIFFERENTIAL;
		}
		else {
			dif = (Differential*)bi->formationInfo;
		}

		dif->acceleration = vec3(0);
		dif->velocity = vec3(cos(thetaBegin), sin(thetaBegin), 0);
		dif->jerk = vec3(0);
		dif->speed = 80 + rand()%10;
		dif->update = Curvilinear;
		bi->lifeTime = standardLifeTime;

		bi->bullet->SetSize(vec2(2, 7));
		thetaBegin += deltaT;
	}
	return true;
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
