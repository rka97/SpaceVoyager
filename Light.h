#pragma once
#include "SceneObject.h"

class Light : public SceneObject
{
public:
	Light(bool isDirectional, const glm::vec3& position = vec3(0.0f, 0.0f, 0.0f), const vec3& scaleVector = vec3(1.0f, 1.0f, 1.0f));
	void SetIntensity(vec3 intensity);
	vec4 GetLightPosition();
private:
	vec4 lightPosition;
	vec3 intensity;
};