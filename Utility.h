#pragma once
#define M_PI	3.14159265358979323846 


extern long long timeNow;

struct KeyboardEvent {
	int code;
	int eventType;
};

struct GravityCenter {
	float mass;
	glm::vec3 pos;
};

struct SceneInfo {
	glm::mat4 View;
	glm::mat4 Projection;
};