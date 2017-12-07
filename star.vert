#version 430 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_color;

layout (location = 0) uniform mat4 Projection = mat4(1);

layout (location = 6) uniform float time;

layout (location = 0) out vec2 starColor;

void main()
{
	vec4 newVertex = in_position;
	newVertex.z += time;
	newVertex.z = fract(newVertex.z);	
	float size = (20.0 * newVertex.z * newVertex.z);
	starColor = smoothstep(1.0, 7.0, size) * color;
	newVertex.z = (999.9 * newVertex.z) - 1000.0;
	gl_Position = Projection * newVertex;
	gl_PointSize = size;
}