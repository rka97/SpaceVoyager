#pragma once
#include "Utility.h"
class Drawable {
public:
	virtual void Draw(SceneInfo& sceneInfo, int numInstances = 0) = 0;
};