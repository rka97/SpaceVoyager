#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const vec3& positionVector, const vec3& scaleVector) : SceneObject(positionVector, scaleVector), fieldOfView(45.0f), zNear(0.1f), zFar(100.0f)
{

}

Camera::~Camera()
{

}

float Camera::GetFieldOfView() const
{
	return fieldOfView;
}

float Camera::GetNearPosition() const
{
	return zNear;
}

float Camera::GetFarPosition() const
{
	return zFar;
}

mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + this->Forward(), Up());
}

mat4 Camera::GetProjectionMatrix(float aspectRatio) const
{
	return glm::perspective(fieldOfView, aspectRatio, zNear, zFar);
}

void Camera::ZoomIn(float angle)
{
	fieldOfView -= angle;
	if (fieldOfView < glm::radians(1.0f))
		fieldOfView = glm::radians(1.0f);
	if (fieldOfView > glm::radians(90.0f))
		fieldOfView = glm::radians(90.0f);
}

void Camera::ZoomOut(float angle)
{
	ZoomIn(-angle);
}

void Camera::SetClippingPlanes(float zNear, float zFar)
{
	if (zNear > 0 && zFar > zNear)
	{
		this->zNear = zNear;
		this->zFar = zFar;
	}
}
