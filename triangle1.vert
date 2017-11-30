#version 430 core

in layout (location = 0) vec3 in_position;
in layout (location = 1) vec4 in_color;
uniform mat4 Model = mat4(1);
uniform mat4 View = mat4(1);
uniform mat4 Projection = mat4(1);
smooth out vec4 color;

void main()
{
	mat4 MVP = Projection * View * Model;
	gl_Position = MVP * vec4(in_position, 1);
	color = in_color;
}