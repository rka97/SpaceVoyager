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

	/* To-do: move this shit to scene or level!! */
	BulletPattern* enemyPattern = new BulletPattern();
	enemyPattern->bulletPositions = new vec3[MAX_NUM_BULLETS];
	enemyPattern->numBullets = MAX_NUM_BULLETS;
	patterns.push_back(enemyPattern);
	BulletPattern* playerPattern = new BulletPattern();
	playerPattern->bulletPositions = new vec3[MAX_PLAYER_BULLETS];
	playerPattern->numBullets = MAX_PLAYER_BULLETS;
	patterns.push_back(playerPattern);
	BulletPattern* explosionPattern = new BulletPattern();
	explosionPattern->bulletPositions = new vec3[MAX_PLAYER_BULLETS];
	explosionPattern->numBullets = MAX_PLAYER_BULLETS;
	patterns.push_back(explosionPattern);
	BulletPattern* sharpnelPattern = new BulletPattern();
	sharpnelPattern->bulletPositions = new vec3[MAX_NUM_BULLETS];
	sharpnelPattern->numBullets = MAX_NUM_BULLETS;
	patterns.push_back(sharpnelPattern);
	BulletPattern* shieldPattern = new BulletPattern();
	shieldPattern->bulletPositions = new vec3[3];
	shieldPattern->numBullets = 3;
	patterns.push_back(shieldPattern);
	/* end shit-to-move */
}

BulletsController::~BulletsController() 
{
	for (auto& pattern : patterns)
	{
		for (auto& bullet : pattern->liveBullets)
		{
			delete bullet;
		}
		delete[] pattern->bulletPositions;
		delete pattern;
	}
	for (auto& bullet : admittedBullets)
		delete bullet;
}

void BulletsController::_Update(vector<BulletInfo*>& bullets, int realdT, float dT) {
	for (int i = 0; i < bullets.size(); i++) 
	{
		BulletInfo* bi = bullets[i];
		switch (bi->type) 
		{
		case DIFFERENTIAL: 
		{
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
		// x [-245, 245] <- [0, 1024], y [-163,163] <- [0, 684]
		auto bulletPos = bi->bullet->Position();
		if (bi->type != EMPTY && (bi->lifeTime <= 0 || fabs(bulletPos.x) > 300 || fabs(bulletPos.y) > 250)) {
			BulletInfo* bi2 = bullets.back();
			bullets[i] = bi2;
			admittedBullets.push_back(bi);
			bullets.pop_back();
			i--;
		}
	}
}

vector<BulletInfo*> BulletsController::GetLiveBullets(int ID)
{
	if (ID < 0 || ID >= patterns.size())
		return vector<BulletInfo*>();
	return patterns[ID]->liveBullets;
}

void BulletsController::Update() {
	int realdT = timeNow - lastUpdateTime;
	float dT = realdT / 500.0f;
	int i = 0;
	// Update all the patterns the bullets controller owns.
	for (auto& pattern : patterns)
	{
		_Update(pattern->liveBullets, realdT, dT);
		if (i == 4 && !pattern->liveBullets.empty())
		{
			pattern->liveBullets[0]->bullet->size *= 1.01f;
			pattern->liveBullets[0]->bullet->outerColor.w *= 0.9f;
		}
		i++;
	}

	lastUpdateTime = timeNow;
}

void BulletsController::Draw(SceneInfo& sceneInfo, int stupid)
{
	// Draw all patterns.
	for (auto& pattern : patterns)
	{
		if (pattern->liveBullets.empty())
			continue;
		int i = 0;
		for (auto& bulletInfo : pattern->liveBullets)
		{
			pattern->bulletPositions[i] = bulletInfo->bullet->Position();
			i++;
		}
		pattern->liveBullets[0]->bullet->GetModel()->InitializeInstanced(pattern->bulletPositions, pattern->liveBullets.size());
		pattern->liveBullets[0]->bullet->Draw(sceneInfo, pattern->liveBullets.size());
	}
}

bool BulletsController::ActivateBullet(int patternID, const std::function<vec3(float&, vec3&, vec3&, vec3&)>& func, float initialT, vec3 center, vec3 rightVector, vec3 upVector)
{
	if (admittedBullets.empty() || patternID >= patterns.size() || patternID < 0) 
		return false;

	auto& pattern = patterns[patternID];
	BulletInfo* bi = admittedBullets.back();
	admittedBullets.pop_back();
	pattern->liveBullets.push_back(bi);

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

bool BulletsController::ActivateBullet(int patternID, std::function<vec3(float, vec3&, vec3&, vec3&, float&)> func,
												vec3 initialPos, vec3 velocity, vec3 acceleration, vec3 jerk, float speed) 
{
	if (admittedBullets.empty() || patternID >= patterns.size() || patternID < 0)
		return false;

	auto& pattern = patterns[patternID];
	BulletInfo* bi = admittedBullets.back();
	admittedBullets.pop_back();
	pattern->liveBullets.push_back(bi);

	Differential* dif = nullptr;
	if (bi->type != DIFFERENTIAL) 
	{
		if (bi->formationInfo != nullptr) delete bi->formationInfo;
		dif = new Differential;
		bi->formationInfo = dif;
		bi->type = DIFFERENTIAL;
	}
	else 
	{
		dif = (Differential*)bi->formationInfo;
	}

	dif->acceleration = acceleration;
	dif->velocity = velocity;
	dif->jerk = jerk;
	dif->speed = speed;
	dif->update = func;

	bi->bullet->SetPosition(initialPos);
	bi->bullet->SetMiddleColor(vec4(1, 0, 0, 1));
	bi->bullet->SetSize(vec2(3, 3));
	bi->lifeTime = standardLifeTime;
	return true;
}

void BulletsController::AddPattern(vector<BulletInfo*> activeBullets, int num)
{
	BulletPattern* pt = new BulletPattern;
	pt->liveBullets = activeBullets;
	pt->numBullets = num;
	pt->bulletPositions = new vec3[num];
	patterns.push_back(pt);
}


// Explosion of bullets
bool BulletsController::PlayerExplosion(vec3 pos, int num, float angle)
{
	if (admittedBullets.size() < num || patterns[2]->liveBullets.size() >= patterns[2]->numBullets)
		return false;

	float thetaBegin = (M_PI - angle) / 2;
	float deltaT = angle / (num - 1);

	for (int i = 0; i < num; i++)
	{
		BulletInfo* bi = admittedBullets.back();
		admittedBullets.pop_back();
		patterns[2]->liveBullets.push_back(bi);
		bi->bullet->SetPosition(pos);
		bi->bullet->SetInnerColor(vec4(0.588, 0.235, 0.572, 1.0));
		bi->bullet->SetOuterColor(vec4(1.0, 1.0, 1.0, 1.0));
		bi->bullet->SetMiddleColor(vec4(0.0, 0.0, 0.0, 0.8));

		Differential* dif = nullptr;
		if (bi->type != DIFFERENTIAL)
		{
			if (bi->formationInfo != nullptr) 
				delete bi->formationInfo;
			dif = new Differential;
			bi->formationInfo = dif;
			bi->type = DIFFERENTIAL;
		}
		else
		{
			dif = (Differential*)bi->formationInfo;
		}

		dif->acceleration = vec3(0);
		dif->velocity = vec3(cos(thetaBegin), sin(thetaBegin), 0);
		dif->jerk = vec3(0);
		dif->speed = 20 + rand() % 10;
		dif->update = Curvilinear;
		bi->lifeTime = standardLifeTime;

		bi->bullet->SetSize(vec2(8, 8));
		thetaBegin += deltaT;
	}
	return true;
}


// Explosion of bullets
bool BulletsController::Shield(vec3 pos, int num, float angle)
{
	if (admittedBullets.size() < num || patterns[4]->liveBullets.size() >= patterns[4]->numBullets)
		return false;

	float thetaBegin = (M_PI - angle) / 2;
	float deltaT = angle / (num - 1);

	for (int i = 0; i < num; i++)
	{
		BulletInfo* bi = admittedBullets.back();
		admittedBullets.pop_back();
		patterns[4]->liveBullets.push_back(bi);
		bi->bullet->SetPosition(pos);
		bi->bullet->SetInnerColor(vec4(0.854, 0.086, 0.286, 1.0));
		bi->bullet->SetOuterColor(1.05f * vec4(1, 1, 1, 1.0));
		bi->bullet->SetMiddleColor(vec4(0.0, 0.0, 0.0, 0.9));

		Differential* dif = nullptr;
		if (bi->type != DIFFERENTIAL)
		{
			if (bi->formationInfo != nullptr)
				delete bi->formationInfo;
			dif = new Differential;
			bi->formationInfo = dif;
			bi->type = DIFFERENTIAL;
		}
		else
		{
			dif = (Differential*)bi->formationInfo;
		}

		dif->acceleration = vec3(0);
		dif->velocity = vec3(cos(thetaBegin), sin(thetaBegin), 0);
		dif->jerk = vec3(0);
		dif->speed = 5 + rand() % 20;
		dif->update = Curvilinear;
		bi->lifeTime = standardLifeTime / 15;

		bi->bullet->SetSize(vec2(20, 20));
		thetaBegin += deltaT;
	}
	return true;
}

// Explosion of bullets
bool BulletsController::BossExplosion(vec3 pos, int num, float angle)
{
	if (admittedBullets.size() < num || patterns[2]->liveBullets.size() >= patterns[2]->numBullets)
		return false;

	float thetaBegin = (M_PI - angle) / 2;
	float deltaT = angle / (num - 1);

	for (int i = 0; i < num; i++)
	{
		BulletInfo* bi = admittedBullets.back();
		admittedBullets.pop_back();
		patterns[2]->liveBullets.push_back(bi);
		bi->bullet->SetPosition(pos);
		bi->bullet->SetInnerColor(vec4(0.113, 0.145, 0.168, 1.0));
		bi->bullet->SetOuterColor(vec4(1.0, 1.0, 1.0, 1.0));
		bi->bullet->SetMiddleColor(vec4(0.0, 0.0, 0.0, 0.7));

		Differential* dif = nullptr;
		if (bi->type != DIFFERENTIAL)
		{
			if (bi->formationInfo != nullptr)
				delete bi->formationInfo;
			dif = new Differential;
			bi->formationInfo = dif;
			bi->type = DIFFERENTIAL;
		}
		else
		{
			dif = (Differential*)bi->formationInfo;
		}

		dif->acceleration = vec3(0);
		dif->velocity = vec3(cos(thetaBegin), sin(thetaBegin), 0);
		dif->jerk = vec3(0);
		dif->speed = 20 + rand() % 10;
		dif->update = Curvilinear;
		bi->lifeTime = standardLifeTime;

		bi->bullet->SetSize(vec2(15, 15));
		thetaBegin += deltaT;
	}
	return true;
}
bool BulletsController::Sharpnel(vec3 pos, int num, float angle)
{
	if (admittedBullets.size() < num || patterns[3]->liveBullets.size() >= patterns[3]->numBullets)
		return false;

	float thetaBegin = (M_PI - angle) / 2;
	float deltaT = angle / (num - 1);

	for (int i = 0; i < num; i++)
	{
		BulletInfo* bi = admittedBullets.back();
		admittedBullets.pop_back();
		patterns[3]->liveBullets.push_back(bi);
		bi->bullet->SetPosition(pos);
		bi->bullet->SetMiddleColor(vec4(0, 0, 0, 0.5f));
		bi->bullet->SetInnerColor(vec4(1.0, 1.0, 1.0, 1.0));
		bi->bullet->SetOuterColor(vec4(1.0, 1.0, 0, 1.0));
		bi->bullet->SetSize(vec2(0.25, 0.75));

		Differential* dif = nullptr;
		if (bi->type != DIFFERENTIAL)
		{
			if (bi->formationInfo != nullptr) delete bi->formationInfo;
			dif = new Differential;
			bi->formationInfo = dif;
			bi->type = DIFFERENTIAL;
		}
		else
		{
			dif = (Differential*)bi->formationInfo;
		}

		dif->acceleration = vec3(0);
		dif->velocity = vec3(cos(thetaBegin), -1.0f * sin(thetaBegin), 0);
		dif->jerk = vec3(0);
		dif->speed = 150 + rand() % 10;
		dif->update = Curvilinear;
		bi->lifeTime = standardLifeTime / 100;

		thetaBegin += deltaT;
	}
	return true;
}

// Standard attack of bullets
bool BulletsController::PlayerAttack(vec3 pos, int num, float angle)
{
	if (admittedBullets.size() < num || patterns[1]->liveBullets.size() >= patterns[1]->numBullets)
		return false;

	float thetaBegin = (M_PI - angle) / 2;
	float deltaT = angle / (num - 1);
	
	for (int i = 0; i < num; i++) 
	{
		BulletInfo* bi = admittedBullets.back();
		admittedBullets.pop_back();
		patterns[1]->liveBullets.push_back(bi);
		bi->bullet->SetPosition(pos);
		bi->bullet->SetMiddleColor(vec4(0, 0, 0, 0.75f));
		bi->bullet->SetInnerColor(vec4(1.0, 1.0, 1.0, 1.0));
		bi->bullet->SetOuterColor(vec4(0.282, 0.917, 0.447, 1.0));
		bi->bullet->SetSize(vec2(2, 6));
		
		Differential* dif = nullptr;
		if (bi->type != DIFFERENTIAL) 
		{
			if (bi->formationInfo != nullptr) delete bi->formationInfo;
			dif = new Differential;
			bi->formationInfo = dif;
			bi->type = DIFFERENTIAL;
		}
		else 
		{
			dif = (Differential*)bi->formationInfo;
		}

		dif->acceleration = vec3(0);
		dif->velocity = vec3(cos(thetaBegin), sin(thetaBegin), 0);
		dif->jerk = vec3(0);
		dif->speed = 80 + rand()%10;
		dif->update = Curvilinear;
		bi->lifeTime = standardLifeTime;

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
