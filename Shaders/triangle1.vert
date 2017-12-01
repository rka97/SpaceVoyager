#version 430 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoordinates;

layout (location = 0) uniform mat4 Model = mat4(1);
layout (location = 4) uniform mat4 View = mat4(1);
layout (location = 8) uniform mat4 Projection = mat4(1);

smooth out vec4 color;
out vec2 texCoordinates;

void main()
{
	texCoordinates = in_texCoordinates;
	mat4 MVP = Projection * View * Model;
	gl_Position = MVP * vec4(in_position, 1);
	color = in_color;
}