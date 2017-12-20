#include "SpaceShip.h"


SpaceShip::SpaceShip() : SceneActor()
{
	Heart = new Bullet(this->position + vec3(0, 4.0f, 5.0f), 0, 1.0f, 2.0f, vec4(0, 1, 0, 1), vec4(0.0f, 0.0f, 0.0f, 0.2f), vec4(1, 1, 1, 1), vec2(2.5, 2.5));
	maxTicksToShield = 600;
}
SpaceShip::~SpaceShip()
{
	if (Heart != nullptr)
		delete Heart;
}
void SpaceShip::Draw(SceneInfo& sceneInfo, int numInstances)
{
	this->SceneActor::Draw(sceneInfo, numInstances);
}
bool SpaceShip::CheckCollision(SceneActor* SA)
{
	Rectangle2D Me = Heart->GetEnclosingRectangle();
	Rectangle2D other = SA->GetEnclosingRectangle();
	return areColliding(Me, other);	
}
void SpaceShip::SetHeartModel(Model* M)
{
	Heart->SetModel(M);
}
void SpaceShip::SetPosition(vec3 pos)
{
	Heart->SetPosition(pos + vec3(0, 0, 5.0f));
	this->SceneObject::SetPosition(pos);
}

Bullet * SpaceShip::GetHeart()
{
	return Heart;
}
