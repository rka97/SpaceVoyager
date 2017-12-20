#include"Bullet.h"

class SpaceShip: public SceneActor
{
	Bullet* Heart;
public:
	SpaceShip();
	~SpaceShip();
	void Draw(SceneInfo& sceneInfo, int numInstances);
	bool CheckCollision(SceneActor*);
	void SetHeartModel(Model* M);
	virtual void SetPosition(vec3 pos);
	Bullet* GetHeart();
	bool dead = false;
	int ticksToShield = 0;
	int maxTicksToShield = 1024;
};