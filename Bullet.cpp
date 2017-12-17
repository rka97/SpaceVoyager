#include "Bullet.h"

int Bullet::PROJECTION = -1;
int Bullet::VIEW = -1;
int Bullet::SIZE = -1;
int Bullet::INNER_RADIUS = -1;
int Bullet::MIDDLE_RADIUS = -1;
int Bullet::OUTER_RADIUS = -1;
int Bullet::INNER_COLOR = -1;
int Bullet::MIDDLE_COLOR = -1;
int Bullet::OUTER_COLOR = -1;

Bullet::Bullet(vec3 position, float innerRadius, float middleRadius, float outerRadius, glm::vec4 innerColor,
	glm::vec4 middleColor, glm::vec4 outerColor, glm::vec2 size)
: SceneActor(position) { 
	this->innerRadius = innerRadius;
	this->middleRadius = middleRadius;
	this->outerRadius = outerRadius;
	this->innerColor = innerColor;
	this->middleColor = middleColor;
	this->outerColor = outerColor;
	this->size = size;
}

Bullet::~Bullet() { }

bool Bullet::SetModel(Model* actorModel){
	if (actorModel == nullptr) return false;
	this->model = actorModel;
	if (PROJECTION == -1) {
		PROJECTION = model->GetParameterId("Projection");
		VIEW = model->GetParameterId("View");
		SIZE = model->GetParameterId("Size");
		INNER_RADIUS = model->GetParameterId("InnerRadius");
		MIDDLE_RADIUS = model->GetParameterId("MiddleRadius");
		OUTER_RADIUS = model->GetParameterId("OuterRadius");
		INNER_COLOR = model->GetParameterId("InnerColor");
		MIDDLE_COLOR = model->GetParameterId("MiddleColor");
		OUTER_COLOR = model->GetParameterId("OuterColor");
	}
	return true;
}

void Bullet::Draw(SceneInfo& sceneInfo, int numInstances) {
	SetParameterValue(PROJECTION, &sceneInfo.projection);
	SetParameterValue(VIEW, &sceneInfo.view);
	SetParameterValue(INNER_RADIUS, &innerRadius);
	SetParameterValue(MIDDLE_RADIUS, &middleRadius);
	SetParameterValue(INNER_COLOR, &innerColor);
	SetParameterValue(MIDDLE_COLOR, &middleColor);
	SetParameterValue(SIZE, &size);
	SetParameterValue(OUTER_COLOR, &outerColor);
	SetParameterValue(OUTER_RADIUS, &outerRadius);
	model->Draw(numInstances);
}


void Bullet::SetInnerRadius(float innerRadius) {
	this->innerRadius = innerRadius;
}
void Bullet::SetOuterRadius(float outerRadius) {
	this->outerRadius = outerRadius;
}
void Bullet::SetInnerColor(glm::vec4 innerColor) {
	this->innerColor = innerColor;
}
void Bullet::SetOuterColor(glm::vec4 outerColor) {
	this->outerColor = outerColor;
}

void Bullet::SetMiddleColor(glm::vec4 middleColor) {
	this->middleColor = middleColor;
}

void Bullet::SetSize(glm::vec2 size)
{
	this->size = size;
}
