#pragma once
#include "Bullet.h"
#include "Utility.h"
#include <chrono>
#include "Drawable.h"
#include <functional>
#define MAX_NUM_BULLETS 1000

class BulletsController : public Drawable
{
private:
	struct BulletInfo;
public:

	enum FormationType {
		DIFFERENTIAL, PARAMETRIC
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

	void Draw(SceneInfo& sceneInfo, int stupid = 0);

private:
	glm::vec3 center;
	vec3* bulletPositions;
	vector<BulletInfo*> admittedBullets;
	vector<BulletInfo*> liveBullets;
	vector<BulletInfo*> deadBullets;
	float constantVelocity = -1;
	long long lastUpdateTime = 0;
	const static int standardLifeTime = 2000;
	bool firstBullet = true;
	struct BulletInfo {
		Bullet* bullet;
		FormationType type;
		void* formationInfo = nullptr;
		int lifeTime = standardLifeTime;
	};
};

