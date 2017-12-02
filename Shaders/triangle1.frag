#version 430 core

layout (location = 0) in vec2 texCoordinates;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;

struct LightInfo
{
	vec4 position;
	vec3 intensity;
};

struct MaterialInfo
{
	vec3 Ka; // Ambient reflectivity
	vec3 Kd; // Diffuse reflectivity
};

layout (location = 12) uniform sampler2D texture_diffuse1;
layout (location = 13) uniform LightInfo light; // will take 13 and 14.
layout (location = 15) uniform MaterialInfo material; // will take 15 and 16.

layout (location = 0) out vec4 out_color;

const int levels = 3;
const float scaleFactor = 1.0 / levels;

vec3 ToonShade()
{
	vec3 s = normalize (light.position.xyz - position.xyz);
	float cosine = max ( 0.0, dot (s, normal) );
	vec3 diffuse = material.Kd * floor (cosine * levels) * scaleFactor;

	return light.intensity * (material.Ka + diffuse);
}

void main()
{
	vec4 texColor = texture(texture_diffuse1, texCoordinates); 
	out_color = vec4(ToonShade(), 1.0) * texColor;
}