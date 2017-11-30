#pragma once
#include <string>
#include <GLFW\glfw3.h>
using namespace std;

class Window
{
	int offsetX;
	int offsetY;
	int width;
	int height;
	string title;
	GLFWwindow* window;
	bool shouldClose;
public:
	Window(int offsetX, int offsetY, int width, int height, string title);
	bool Initialize();
	void Resize(int width, int height);
	GLFWwindow* GetGLFWPointer();
	bool ShouldClose();
	void SetClose(bool shouldClose);
	void WindowLoop();
	void Terminate();
	~Window();
};

