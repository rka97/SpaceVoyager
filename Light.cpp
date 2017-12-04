#include "Light.h"

Light::Light(bool isDirectional, const glm::vec3& position, const glm::vec3& scaleVector) : SceneObject(position, scaleVector)
{
	lightPosition.w = (float)isDirectional;
}


void Light::SetIntensity(vec3 intensity)
{
	this->intensity = intensity;
}

vec4 Light::GetLightPosition()
{
	lightPosition.x = position.x;
	lightPosition.y = position.y;
	lightPosition.z = position.z;
	return lightPosition;
}
