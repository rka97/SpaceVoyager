#pragma once
#include "Bullet.h"
#include "Utility.h"
#include <chrono>
#include "Drawable.h"
#include <functional>
#include <glm/gtx/norm.hpp>

#define MAX_NUM_BULLETS 1500
#define MAX_PLAYER_BULLETS 400

class BulletsController : public Drawable
{
private:
	struct BulletInfo;
public:

	enum FormationType {
		DIFFERENTIAL, PARAMETRIC, EMPTY
	};

	struct Differential {
		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 jerk;
		float speed;
		std::function<vec3(float, vec3&, vec3&, vec3&, float&)> update;

		Differential(vec3 velocity, vec3 acceleration, vec3 jerk, float speed, std::function<vec3(float, vec3&, vec3&, vec3&, float&)> func) {
			this->velocity = velocity;
			this->acceleration = acceleration;
			this->jerk = jerk;
			this->speed = speed;
			this->update = func;
		}
		Differential(){}
	};

	struct Parametric {
		float t = 0;
		vec3 center = vec3(0);
		vec3 rightVector = vec3(1, 0, 0);
		vec3 upVector = vec3(0, 1, 0);
		std::function<vec3(float&, vec3&, vec3&, vec3&)> update;
		Parametric(float t, vec3 center, vec3 rightVector, vec3 upVector, std::function<vec3(float&, vec3&, vec3&, vec3&)> func) {
			this->t = t;
			this->center = center;
			this->rightVector = rightVector;
			this->upVector = upVector;
			this->update = func;
		}
		Parametric(){}
	};

	BulletsController();
	~BulletsController();
	void SetCenter(glm::vec3 center);
	void Update();
	void AddBullets(vector<Bullet*> bullets);
	
	bool ActivateBullet(const std::function<vec3(float&, vec3&, vec3&, vec3&)>&, float initialT = 0, vec3 center = vec3(0), vec3 rightVector = vec3(1, 0, 0), vec3 upVector = vec3(0, 1, 0));
	bool ActivateBullet(std::function<vec3(float, vec3&, vec3&, vec3&, float&)> func, vec3 initialPos, vec3 velocity, vec3 acceleration, vec3 jerk, float speed = -1);
	bool ActivateShield(vec3 pos, vec3 velocity, vec3 acceleration, float initialSize, float maximumSize);
	bool PlayerAttack(vec3 pos, int num, float angle);

	void Draw(SceneInfo& sceneInfo, int stupid = 0);

private:
	void _Update(vector<BulletInfo*>& bullets, int realdT, float dT);
	glm::vec3 center;
	vec3* bulletPositions;
	vec3* playerBulletPositions;
	vec3* shieldPos;
	vector<BulletInfo*> admittedBullets;
	vector<BulletInfo*> liveBullets;
	vector<BulletInfo*> playerLiveBullets;
	BulletInfo* shield = nullptr;
	long long lastUpdateTime = 0;
	const static int standardLifeTime = 10000;
	bool firstBullet = true;

	struct BulletInfo {
		Bullet* bullet;
		FormationType type = EMPTY;
		void* formationInfo = nullptr;
		int maxLifeTime;
		int lifeTime = standardLifeTime;
		float initialSize;
		float maximumSize;
	};
};

