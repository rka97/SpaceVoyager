#version 430 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoordinates;

layout (location = 0) uniform mat4 ModelView = mat4(1);
layout (location = 1) uniform mat4 Projection = mat4(1);
layout (location = 2) uniform mat4 ModelViewNormal = mat4(1);
layout (location = 3) uniform mat4 ModelViewProjection = mat4(1);

layout (location = 0) out vec2 texCoordinates;
layout (location = 1) out vec3 position;
layout (location = 2) out vec3 normal;

void main()
{
	normal = vec3(normalize(ModelViewNormal * vec4(in_normal, 0.0)));
	position = vec3(ModelView * vec4(in_position, 1.0));
	texCoordinates = in_texCoordinates;
	gl_Position = Projection * vec4(position, 1.0);
}