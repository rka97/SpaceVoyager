#include <iostream>
#include "Window.h"
using namespace std;

Window::Window(int offsetX, int offsetY, int width, int height, string title) : offsetX(offsetX), offsetY(offsetY), width(width), height(height), title(title)
{
}

bool Window::Initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window) {
		cout << "Failed to create GLFW window.\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, offsetX, offsetY);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Resize(this->width, this->height);
	return true;
}

void Window::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
	glViewport(0, 0, width, height);
}

GLFWwindow * Window::GetGLFWPointer()
{
	return window;
}

bool Window::ShouldClose()
{
	shouldClose = shouldClose || glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window);
	return shouldClose;
}

void Window::SetClose(bool shouldClose)
{
	this->shouldClose = shouldClose;
}

void Window::WindowLoop()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::Terminate()
{
	glfwTerminate();
}


Window::~Window()
{
}
