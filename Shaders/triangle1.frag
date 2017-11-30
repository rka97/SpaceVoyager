#version 430 core

smooth in vec4 color;
out layout (location = 0) vec4 out_color;

void main()
{
	out_color = color;
}