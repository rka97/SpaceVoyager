#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 1) uniform mat4 Model = mat4(1);
layout (location = 2) uniform mat4 View = mat4(1);
layout (location = 3) uniform mat4 Projection = mat4(1);
smooth out vec4 color;

void main()
{
	mat4 MVP = Projection * View * Model;
	gl_Position = MVP * vec4(in_position, 1);
	color = in_color;
}