#pragma once
#include <list>
#include <string>
#include <glm\glm.hpp>
using namespace std;

// A struct used to OpenGL Context Information.
struct GLContextInfo
{
	struct {
		GLint Major;
		GLint Minor;
		string Driver;
		string ShadingLanguage;
	} Version; // Note: this declares A struct with the name version whose type has no name (i.e. can not be re-used).
	string Vendor;
	string Renderer;
	list<string> SupportedExtensions;
	list<string> SupportedGLSLVersions;
};

class Graphics
{
	glm::uvec2 frameBufferSize;
public:
	Graphics();
	~Graphics();
	
	// Render functions: responsible for drawing all objects in the main loop.
	void Render();
	void DirtyRender();
	void DirtyInitialize();

	// Initialize function: responsible for initializing everything to defaults.
	void Initialize(unsigned int width, unsigned int height);
	
	// Resizes the OpenGL Viewport.
	void Resize(unsigned int width, unsigned int height);

	// Print OpenGL Context Information to the standard output buffer.
	void PrintContextInfo();

private:
	// Checks for OpenGL Errors (who could have guessed?)
	void CheckForErrors();
	
	// Get the OpenGL Context Information.
	GLContextInfo GetContextInfo();
};