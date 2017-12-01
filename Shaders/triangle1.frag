#version 430 core

layout (location = 2) in vec2 texCoordinates;
layout (location = 0) out vec4 out_color;
layout (location = 12) uniform sampler2D texture_diffuse1;

void main()
{
	out_color = texture(texture_diffuse1, texCoordinates);;
}