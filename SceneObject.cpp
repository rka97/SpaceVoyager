#include "SceneObject.h"
#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define DEFAULT_DIRECTION_FORWARD (vec3(0, 0, 1))
#define DEFAULT_DIRECTION_UP (vec3(0, 1, 0))
#define DEFAULT_DIRECTION_RIGHT (vec3(1, 0, 0))

SceneObject::SceneObject(const vec3& positionVector, const vec3& rotationVector, const vec3& scaleVector) : position(positionVector), rotation(quat(vec4(rotationVector, 1))), scale(scaleVector)
{

}

SceneObject::~SceneObject()
{

}

vec3 SceneObject::Position() const
{
	return position;
}

vec3 SceneObject::Forward() const
{
	return rotate(rotation, DEFAULT_DIRECTION_FORWARD);
}

vec3 SceneObject::Backward() const
{
	return -Forward();
}

vec3 SceneObject::Right() const
{
	return rotate(rotation, DEFAULT_DIRECTION_RIGHT);
}

vec3 SceneObject::Left() const
{
	return -Right();
}


vec3 SceneObject::Up() const
{
	return rotate(rotation, DEFAULT_DIRECTION_UP);
}

vec3 SceneObject::Down() const
{
	return -Up();
}

void SceneObject::SetPosition(const vec3 & positionVector)
{
	this->position = positionVector;
}

void SceneObject::SetScale(const vec3 & scaleVector)
{
	this->scale = scaleVector;
}

void SceneObject::SetRotation(const vec3 & rotationVector)
{
	rotation = quat(vec4(rotationVector, 1));
}

mat4 SceneObject::GetTransformationMatrix() const
{
	return translate(position) * toMat4(rotation) * glm::scale(scale);
}

mat4 SceneObject::GetRotationMatrix()
{
	return toMat4(rotation);
}

void SceneObject::Move(const vec3 & direction)
{
	position += direction;
}

void SceneObject::Rotate(const vec3 & direction, float angle)
{
	rotation = rotate(rotation, angle, direction);
}

void SceneObject::RotateAroundRight(float angle)
{
	rotation = rotate(rotation, angle, DEFAULT_DIRECTION_RIGHT);
}

void SceneObject::RotateAroundUp(float angle)
{
	rotation = rotate(this->rotation, angle, DEFAULT_DIRECTION_UP);
}

void SceneObject::RotateAroundForward(float angle)
{
	rotation = rotate(rotation, angle, DEFAULT_DIRECTION_FORWARD);
}