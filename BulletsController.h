#pragma once
#include "Bullet.h"
#include "Utility.h"
#include <chrono>
#include "Drawable.h"
#include <functional>
#include <glm/gtx/norm.hpp>
#include "BulletsControllerHelper.h"

#define MAX_NUM_BULLETS 1500
#define MAX_PLAYER_BULLETS 400

class BulletsController : public Drawable
{
private:
public:
	BulletsController();
	~BulletsController();
	void SetCenter(glm::vec3 center);
	void Update();
	void AddBullets(vector<Bullet*> bullets);
	
	bool ActivateBullet(int patternID, const std::function<vec3(float&, vec3&, vec3&, vec3&)>&, float initialT = 0, vec3 center = vec3(0), vec3 rightVector = vec3(1, 0, 0), vec3 upVector = vec3(0, 1, 0));
	bool ActivateBullet(int patternID, std::function<vec3(float, vec3&, vec3&, vec3&, float&)> func, vec3 initialPos, vec3 velocity, vec3 acceleration, vec3 jerk, float speed = -1);

	bool PlayerAttack(vec3 pos, int num, float angle);
	bool Explosion(vec3 pos, int num, float angle);

	void Draw(SceneInfo& sceneInfo, int stupid = 0);

private:
	void _Update(vector<BulletInfo*>& bullets, int realdT, float dT);
	glm::vec3 center;
	vector<BulletInfo*> admittedBullets;
	vector<BulletPattern*> patterns;

	long long lastUpdateTime = 0;
	const static int standardLifeTime = 10000;
	bool firstBullet = true;
};

