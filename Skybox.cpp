#include "Skybox.h"

Skybox::Skybox(const string & name, const string & path, Material * mat, bool gamma) : Model(name, path, mat, gamma)
{

}

void Skybox::InitializeSkybox()
{
	this->Initialize();
}
