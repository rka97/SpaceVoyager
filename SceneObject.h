#pragma once
#include <glm\glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtc\quaternion.hpp>
using namespace glm;

class SceneObject
{
public:
	SceneObject(const vec3& positionVector = vec3(0.0f, 0.0f, 0.0f), const vec3& scaleVector = vec3(1.0f, 1.0f, 1.0f));
	~SceneObject();

	vec3 Position() const;
	vec3 Forward() const;
	vec3 Backward() const;
	vec3 Right() const;
	vec3 Left() const;
	vec3 Up() const;
	vec3 Down() const;

	virtual void SetPosition(const vec3& positionVector);
	void SetScale(const vec3& scaleVector);
	void SetRotation(const vec3& rotationVector);

	mat4 GetTransformationMatrix() const;
	mat4 GetRotationMatrix();
	void Move(const vec3& direction);
	void Rotate(const vec3& direction, float angle);
	void RotateAroundRight(float angle);
	void RotateAroundUp(float angle);
	void RotateAroundForward(float angle);

protected:
	vec3 position;
	vec3 scale;
	quat rotation;
};
