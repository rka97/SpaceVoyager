#pragma once
#define M_PI	3.14159265358979323846 

#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"

extern long long timeNow;

struct MouseChange
{
	glm::vec2 position;
	glm::vec2 prevPosition;
	bool changed;
};

struct KeyboardEvent {
	int code;
	int eventType;
};

struct SceneInfo {
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec4 lightPosition;
	glm::vec3 lightIntensity;
	glm::vec3 cameraPosition;
};

struct Box {
	glm::vec3 vertices[8];
	Box(){}
	Box(glm::vec3 a, glm::vec3 b) {
		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = glm::vec3(a.x, a.y, b.z);
		vertices[3] = glm::vec3(a.x, b.y, a.z);
		vertices[4] = glm::vec3(b.x, a.y, a.z);
		vertices[5] = glm::vec3(b.x, b.y, a.z);
		vertices[6] = glm::vec3(b.x, a.y, b.z);
		vertices[7] = glm::vec3(a.x, b.y, b.z);
	}
};

struct Rectangle2D {
	glm::vec2 min, max;
	Rectangle2D(glm::vec2 a, glm::vec2 b) {
		this->min = a;
		this->max = b;
	}
	Rectangle2D() {}
};

static glm::vec3 Curvilinear(float dT, glm::vec3& velocity, glm::vec3& acceleration, glm::vec3& jerk, float& speed) {
	float velocityNorm = glm::l2Norm(velocity);
	glm::vec3 tangent = velocity * (1 / velocityNorm);
	glm::vec3 normal = glm::vec3(tangent.y, -tangent.x, 0);
	acceleration += dT * jerk;
	velocity += (tangent * acceleration.x + normal * acceleration.y) * dT;
	if (speed != -1)
		return dT * (velocity * speed / velocityNorm);
	return dT * velocity;
}

static bool areColliding(Rectangle2D& f, Rectangle2D& s) {
	// If one rectangle is on left side of other
	if (f.min.x > s.max.x || s.min.x > f.max.x)
		return false;

	// If one rectangle is above other
	if (f.min.y > s.max.y || s.min.y > f.max.y)
		return false;

	return true;
}
