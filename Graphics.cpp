#define GLEW_STATIC
#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Graphics.h"
#include "ShaderProgram.h"
#include "Model.h"
#include <iostream>
using namespace std;
using glm::vec3;
using glm::vec4;

void Graphics::DirtyInitialize()
{
	glEnable(GL_MULTISAMPLE);

	Light* mainLight = scene->GetSceneLight();


	CheckForErrors();
}


void Graphics::DirtyRender()
{
	Camera* cam = scene->GetSceneCamera();
	Light* mainLight = scene->GetSceneLight();
	SceneActor* playerShip = scene->GetActor("Player");

	/* draw triangles */
	float aspectratio = (float)frameBufferSize.x / frameBufferSize.y;

	glm::mat4 Model = playerShip->GetTransformationMatrix();
	mat4 View = cam->GetViewMatrix();
	mat4 ModelView = View * Model;
	mat4 ModelViewNormal = glm::transpose(glm::inverse(ModelView));
	mat4 ModelNormal = glm::transpose(glm::inverse(Model));
	mat4 Projection = cam->GetProjectionMatrix(aspectratio);
	mat4 ModelViewProjection = Projection * ModelView;

	playerShip->SetParameterValue("Model", ((void*)&Model));
	playerShip->SetParameterValue("ModelView", ((void*)&ModelView));
	playerShip->SetParameterValue("Projection", ((void*)&Projection));
	playerShip->SetParameterValue("ModelViewNormal", ((void*)&ModelViewNormal));
	playerShip->SetParameterValue("ModelNormal", ((void*)&ModelNormal));
	playerShip->SetParameterValue("ModelViewProjection", ((void*)&ModelViewProjection));
	playerShip->SetParameterValue("cameraPosition", ((void*)&cam->Position()));

	vec4 lightPosition = mainLight->GetLightPosition();
	playerShip->SetParameterValue("light.position", &lightPosition);
	playerShip->SetParameterValue("light.intensity", &(glm::vec3(1.5f)));

	/* draw model */
	playerShip->Draw();

	glUseProgram(0);
	CheckForErrors();
}


Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DirtyRender();
}

void Graphics::Initialize(unsigned int width, unsigned int height, Scene* scene)
{
	PrintContextInfo();
	Resize(width, height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	if (scene == nullptr)
	{
		cout << "Error in Graphics::Initialize(int, int, Scene*): can't initialize with NULL scene!\n";
		return;
	}
	this->scene = scene;
	DirtyInitialize();
}

void Graphics::Resize(unsigned int width, unsigned int height)
{
	frameBufferSize = glm::uvec2(width, height);
	glViewport(0, 0, width, height);
}

void Graphics::CheckForErrors()
{
	for (GLenum error; (error = glGetError()) != GL_NO_ERROR; )
	{
		cout << "OpenGL Error: \t";
		if (error == GL_INVALID_ENUM)
			cout << "GL_INVALID_ENUM";
		if (error == GL_INVALID_VALUE)
			cout << "GL_INVALID_VALUE";
		if (error == GL_INVALID_OPERATION)
			cout << "GL_INVALID_OPERATION";
		if (error == GL_STACK_OVERFLOW)
			cout << "GL_STACK_OVERFLOW";
		if (error == GL_STACK_UNDERFLOW)
			cout << "GL_STACK_UNDERFLOW";
		if (error == GL_OUT_OF_MEMORY)
			cout << "GL_OUT_OF_MEMORY";
		if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION";
		if (error == GL_CONTEXT_LOST)
			cout << "GL_CONTEXT_LOST";
		cout << "\n";
		std::cin.get();
	}
}

GLContextInfo Graphics::GetContextInfo()
{
	GLContextInfo infos;
	glGetIntegerv(GL_MAJOR_VERSION, &infos.Version.Major);
	glGetIntegerv(GL_MINOR_VERSION, &infos.Version.Minor);
	infos.Version.Driver = (const char*)glGetString(GL_VERSION);
	infos.Version.ShadingLanguage = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	infos.Vendor = (const char*)glGetString(GL_VENDOR);
	infos.Renderer = (const char*)glGetString(GL_RENDERER);

	GLint extensionCount = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
	for (int i = 0; i < extensionCount; i++)
	{
		infos.SupportedExtensions.push_back((const char*)glGetStringi(GL_EXTENSIONS, i));
	}

	GLint supportedGLSLCount = 0;
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &supportedGLSLCount);
	for (int i = 0; i < supportedGLSLCount; i++)
	{
		infos.SupportedGLSLVersions.push_back((const char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION, i));
	}

	return infos;
}

void Graphics::PrintContextInfo()
{
	GLContextInfo infos = Graphics::GetContextInfo();
	cout << "OpenGL version: " << infos.Version.Major << "." << infos.Version.Minor << "\n";
	cout << "GLSL version: " << infos.Version.ShadingLanguage << "\n";
	cout << "Renderer: " << infos.Renderer << "\n";
	cout << "Vendor: " << infos.Vendor << "\n";
	cout << "Driver version: " << infos.Version.Driver << "\n";
}
