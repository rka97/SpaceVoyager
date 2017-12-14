#pragma once
#include "Model.h"
#include <math.h>
#include "Utility.h"

class BulletModel : public Model
{
public:
	BulletModel(const string& name, Material* mat);
	~BulletModel();
	void UpdateMesh();
};

