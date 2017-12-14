#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics.h"
#include "ShaderProgram.h"
#include "Model.h"
#include <iostream>

using namespace std;
using glm::vec3;
using glm::vec4;

SceneInfo sceneInfo;

void Graphics::DirtyInitialize()
{
}


void Graphics::DrawScene()
{
	Camera* cam = scene->GetSceneCamera();
	Light* mainLight = scene->GetSceneLight();
	map<string, Drawable*> sceneActors = *(scene->GetSceneActors());
	float aspectratio = (float)frameBufferSize.x / frameBufferSize.y;
	mat4 view = cam->GetViewMatrix();
	vec4 lightPosition = mainLight->GetLightPosition();
	vec3 lightIntensity = vec3(1.0f);
	vec3 cameraPosition = cam->Position();

	mat4 projection = cam->GetProjectionMatrix(aspectratio);
	sceneInfo.view = view;
	sceneInfo.projection = projection;
	sceneInfo.lightPosition = mainLight->GetLightPosition();
	sceneInfo.lightIntensity = vec3(1.0f);

	for (map<string, Drawable*>::iterator it = sceneActors.begin(); it != sceneActors.end(); it++)
	{
		Drawable* currentActor = it->second;
		currentActor->Draw(sceneInfo);
	}
}

void Graphics::DirtyRender()
{
	DrawScene();
	//glUseProgram(0);
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
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (scene == nullptr)
	{
		cout << "Error in Graphics::Initialize(int, int, Scene*): can't initialize with NULL scene!\n";
		return;
	}
	this->scene = scene;
	glEnable(GL_MULTISAMPLE);
	DirtyInitialize();
	CheckForErrors();
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
