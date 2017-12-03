#pragma once

#include "SceneObject.h"

class Camera : public SceneObject
{
public:
	Camera(const vec3& positionVector = vec3(0, 0, 0), const vec3& rotationVector = vec3(0, 0, 0), const vec3& scaleVector = vec3(1.0f, 1.0f, 1.0f));
	virtual ~Camera();

	float GetFieldOfView() const;
	float GetNearPosition() const;
	float GetFarPosition() const;

	mat4 GetViewMatrix() const;
	mat4 GetProjectionMatrix(float aspectRatio) const;

	void ZoomIn(float angle);
	void ZoomOut(float angle);
	void SetClippingPlanes(float zNear, float zFar);

protected:
	float fieldOfView;
	float zNear;
	float zFar;
};