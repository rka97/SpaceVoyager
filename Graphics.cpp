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

ShaderProgram prog;
GLuint vertexarray = 0;
GLuint vertexbuffer = 0;

#include "Camera.h"
Material* mat;
Model* shipModel;

void Graphics::DirtyInitialize()
{
	glEnable(GL_MULTISAMPLE);

	prog.Initialize();
	prog.AddAndCompileShader("Shaders\\triangle1.vert", 'v');
	prog.AddAndCompileShader("Shaders\\triangle1.frag", 'f');
	prog.LinkProgram();

	prog.AddParameter("ModelView", 0, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	prog.AddParameter("Projection", 1, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	prog.AddParameter("ModelViewNormal", 2, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	prog.AddParameter("ModelViewProjection", 3, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	prog.AddParameter("texture_diffuse1", 12, 1, SP_SAMPLER2D, GLSL_VAR_UNIFORM);
	prog.AddParameter("light.position", 13, 1, SP_VEC4, GLSL_VAR_UNIFORM);
	prog.AddParameter("light.intensity", 14, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	prog.AddParameter("material.Ka", 15, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	prog.AddParameter("material.Kd", 16, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	prog.AddParameter("material.Ks", 17, 1, SP_VEC3, GLSL_VAR_UNIFORM);
	prog.AddParameter("material.Shininess", 18, 1, SP_FLOAT, GLSL_VAR_UNIFORM);

	prog.AddParameter("in_position", 0, 1, SP_VEC3, GLSL_VAR_IN);
	prog.AddParameter("in_normal", 1, 1, SP_VEC3, GLSL_VAR_IN);
	prog.AddParameter("in_texCoordinates", 2, 1, SP_VEC2, GLSL_VAR_IN);

	mat = new Material("Ship Material", &prog);
	mat->Initialize();

	shipModel = new Model("Ship", "corvette/Corvette-F3.obj", mat, false);

	Camera* cam = scene->GetSceneCamera();

	/* put the camera at the positive z-axis */
	cam->SetPosition(glm::vec3(0, 0, 20));

	/* turn the camera back to the origin */
	cam->RotateAroundUp(glm::radians(180.0f));
	
	cam->ZoomIn(glm::radians(40.0f));

	CheckForErrors();
}


void Graphics::DirtyRender()
{
	Camera* cam = scene->GetSceneCamera();
	Light* mainLight = scene->GetSceneLight();
	/* draw triangles */
	float aspectratio = (float)frameBufferSize.x / frameBufferSize.y;

	SceneObject spaceShipSceneObject;
	spaceShipSceneObject.SetPosition(vec3(0.0f, 0.0f, 0.0f));
	spaceShipSceneObject.SetScale(vec3(0.004f));
	spaceShipSceneObject.RotateAroundRight(glm::radians(90.0f));
	glm::mat4 Model = spaceShipSceneObject.GetTransformationMatrix();
	static float i = 0.0f;
	i--;
	mat4 View = cam->GetViewMatrix();
	mat4 ModelView = View * Model;
	mat4 ModelViewNormal = glm::transpose(glm::inverse(ModelView));
	mat4 Projection = cam->GetProjectionMatrix(aspectratio);
	mat4 ModelViewProjection = Projection * ModelView;

	prog.SetParameter("ModelView", ((void*)&ModelView));
	prog.SetParameter("Projection", ((void*)&Projection));
	prog.SetParameter("ModelViewNormal", ((void*)&ModelViewNormal));
	prog.SetParameter("ModelViewProjection", ((void*)&ModelViewProjection));
	//vec3 lightPosition = vec3(View * vec4(10.0f * sin(i / 500), -10.0f, 10.0f * sin(i / 500), 1.0f));
	//mainLight->SetPosition(spaceShipSceneObject.Position() + spaceShipSceneObject.Up());
	//lightPosition = lightPosition + vec3(1.5*sin(i / 50) * 1.0f, 2 + 2*cos(i / 50), 1.0f);
	vec4 lightPosition = View * mainLight->GetLightPosition();
	prog.SetParameter("light.position", &lightPosition);
	prog.SetParameter("light.intensity", &(glm::vec3(2.0f, 2.0f, 2.0f)));
	prog.SetParameter("material.Ka", &(glm::vec3(0.3f, 0.3f, 0.3f)));
	prog.SetParameter("material.Kd", &(glm::vec3(1.0f, 1.0f, 1.0f)));
	prog.SetParameter("material.Ks", &(glm::vec3(0.2f, 0.2f, 0.2f)));
	prog.SetParameter("material.Shininess", &(glm::vec3(1.0f, 1.0f, 1.0f)));

	/* draw model */
	shipModel->Draw();

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
