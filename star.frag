#version 430 core

layout (location = 0) out vec4 color;

uniform sampler2D tex_star;

layout (location = 0) in vec2 starColor;

void main()
{
	color = starColor * texture(tex_star, gl_PointCoord);
}