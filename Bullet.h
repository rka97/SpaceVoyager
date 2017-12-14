#pragma once
#include "SceneActor.h"
#include "Utility.h"

class Bullet :
	public SceneActor
{
	friend class BulletsController;
public:
	Bullet(vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float innerRadius = 0, float middleRadius = 0.5, float outerRadius = 1, glm::vec4 innerColor = vec4(1,1,1,1), 
		glm::vec4 middleColor = vec4(1,0,0,1) , glm::vec4 outerColor = glm::vec4(1,0,0,0));
	~Bullet();
	void Draw(SceneInfo& sceneInfo, int numInstances = 1);

	void SetInnerRadius(float innerRadius);
	void SetOuterRadius(float outerRadius);
	void SetInnerColor(glm::vec4 innerColor);
	void SetOuterColor(glm::vec4 outerColor);
	bool SetModel(Model* actorModel);

private:
	float innerRadius;
	float middleRadius;
	float outerRadius;
	float holeRadius;
	glm::vec4 innerColor;
	glm::vec4 outerColor;
	glm::vec4 middleColor;
	
	static int SIZE;
	static int INNER_RADIUS;
	static int MIDDLE_RADIUS;
	static int OUTER_RADIUS;
	static int INNER_COLOR;
	static int MIDDLE_COLOR;
	static int OUTER_COLOR;
	static int VIEW;
	static int PROJECTION;
};

