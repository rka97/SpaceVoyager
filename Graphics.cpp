#define GLEW_STATIC
#include <GL\glew.h>
#include "GLFW\glfw3.h"
#include <glm\glm.hpp>
#include "Graphics.h"
#include "ShaderProgram.h"
#include <iostream>
using namespace std;
using glm::vec3;
using glm::vec4;

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	Vertex(const vec3& position, const vec4& color)
		: Position(position), Color(color)
	{}
};

ShaderProgram prog;
GLuint vertexarray = 0;
GLuint vertexbuffer = 0;

GLint uniformlocation_Model = -1;
GLint uniformlocation_View = -1;
GLint uniformlocation_Projection = -1;

#include "Camera.h"
Camera cam;
SceneObject triangle;
void Graphics::DirtyInitialize()
{
	prog.Initialize();
	prog.AddAndCompileShader("Shaders\triangle2.vert", 'v');
	prog.AddAndCompileShader("Shaders\triangle1.frag", 'f');
	prog.LinkProgram();

	prog.AddParameter("Model", 1, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	prog.AddParameter("View", 2, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	prog.AddParameter("Projection", 3, 1, SP_MAT4, GLSL_VAR_UNIFORM);
	prog.AddParameter("in_position", 0, 1, SP_VEC3, GLSL_VAR_IN);
	prog.AddParameter("in_color", 1, 1, SP_VEC4, GLSL_VAR_IN);


	glBindVertexArray(vertexarray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	/* setup vertexbuffer */
	vector<Vertex> vertices;
	vertices.push_back(Vertex(vec3(0, 0, 0), vec4(1, 0, 0, 1)));
	vertices.push_back(Vertex(vec3(1, 0, 0), vec4(0, 1, 0, 1)));
	vertices.push_back(Vertex(vec3(0, 1, 0), vec4(0, 0, 1, 1)));
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* put the camera at the positive z-axis */
	cam.SetPosition(glm::vec3(0, 0, 5));

	/* turn the camera back to the origin */
	cam.RotateAroundUp(glm::radians(180.0f));
}


void Graphics::DirtyRender()
{
	/* draw triangles */

	float aspectratio = (float)frameBufferSize.x / frameBufferSize.y;

	mat4 Model = triangle.GetTransformationMatrix();
	mat4 View = cam.GetViewMatrix();
	mat4 Projection = cam.GetProjectionMatrix(aspectratio);

	prog.SetParameter("Model", ((void*)&Model));
	prog.SetParameter("View", ((void*)&View));
	prog.SetParameter("Projection", ((void*)&Projection));


	/* clear framebuffer */
	glClear(GL_COLOR_BUFFER_BIT);

	/* draw triangle */
	prog.UseProgram();
	glBindVertexArray(vertexarray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glUseProgram(0);
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

void Graphics::Initialize(unsigned int width, unsigned int height)
{
	PrintContextInfo();
	Resize(width, height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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
