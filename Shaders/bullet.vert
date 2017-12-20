#version 430 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 Center_worldspace;

layout (location = 0) uniform mat4 View = mat4(1);
layout (location = 1) uniform mat4 Projection = mat4(1);

layout (location = 2) uniform vec2 Size = vec2(1,1);
layout (location = 3) uniform float Angle = 0;

layout (location = 0) out vec3 position;

void main()
{
	vec4 cameraRight = View[0];
	vec4 cameraUp = View[1];
	vec4 relpos = Size.x * in_position.x * cameraRight + Size.y * in_position.y * cameraUp;
	float c = cos(Angle);
	float s = sin(Angle);
	relpos = vec4(relpos.x * c - relpos.y * s, relpos.x * s + relpos.y * c, in_position.z, 1);
	gl_Position = Projection * View *(vec4(Center_worldspace, 0.0) + relpos);
	//vColor = in_vertexColor;
	position = in_position;
}