#pragma once
#include "Bullet.h"
#define default_standard_lifetime 10000

enum FormationType {
	DIFFERENTIAL, PARAMETRIC, EMPTY
};

struct BulletInfo {
	Bullet* bullet;
	FormationType type = EMPTY;
	void* formationInfo = nullptr;
	int maxLifeTime;
	int lifeTime = default_standard_lifetime;
	float initialSize;
	float maximumSize;
};

struct BulletPattern
{
	vec3* bulletPositions;
	vector<BulletInfo*> liveBullets;
	int numBullets;
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
	Differential() {}
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
	Parametric() {}
};