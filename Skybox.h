#pragma once
#include "Mesh.h"
#include "Model.h"

class Skybox : public Model
{
private:
public:
	Skybox(const string& name, const string& path, Material* mat, bool gamma = false);
	void InitializeSkybox();
	virtual void Draw();
};