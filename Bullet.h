#pragma once
#include "SceneActor.h"
#include "Utility.h"

class Bullet :
	public SceneActor
{
	friend class BulletsController;
public:
	Bullet(vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float innerRadius = 0, float middleRadius = 0.5, float outerRadius = 1, glm::vec4 innerColor = vec4(1,1,1,1), 
		glm::vec4 middleColor = vec4(0.948,0.109,0,1411) , glm::vec4 outerColor = glm::vec4(1,0,0,0), glm::vec2 size = glm::vec2(5));
	~Bullet();
	void Draw(SceneInfo& sceneInfo, int numInstances = 1);
	virtual float GetEnclosingRadius();
	void SetInnerRadius(float innerRadius);
	void SetOuterRadius(float outerRadius);
	void SetInnerColor(glm::vec4 innerColor);
	void SetOuterColor(glm::vec4 outerColor);
	void SetMiddleColor(glm::vec4 middleColor);
	void SetSize(glm::vec2 size);
	bool SetModel(Model* actorModel);
	vec2 GetSize();
	Rectangle2D GetEnclosingRectangle();


private:
	float innerRadius;
	float middleRadius;
	float outerRadius;
	float holeRadius;
	glm::vec4 innerColor;
	glm::vec4 outerColor;
	glm::vec4 middleColor;
	glm::vec2 size;
	
	static int PROJECTION;
	static int VIEW;
	static int CENTER_WORLDSPACE;
	static int SIZE;
	static int INNER_RADIUS;
	static int MIDDLE_RADIUS;
	static int OUTER_RADIUS;
	static int INNER_COLOR;
	static int MIDDLE_COLOR;
	static int OUTER_COLOR;
};

