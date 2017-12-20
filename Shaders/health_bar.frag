#version 430 core

in vec4 gl_FragCoord;

layout (location = 6) uniform int health;
layout (location = 7) uniform int maxHealth;

layout (location = 0) out vec4 fragColor;


void main()
{	
	float current_pos = 1 - gl_FragCoord.x / 1024;
	float curr_health = health * 1.0f / maxHealth;
	
	if (current_pos < curr_health)
		fragColor = vec4(0.870, 0.2, 0.105, 1.0);
	else
		fragColor = vec4(0.5, 1.0, 0.0, 1.0);
}