#version 430 core

varying vec3 position;

layout (location = 4)  uniform float InnerRadius = 0;
layout (location = 5) uniform float MiddleRadius = 0.5;
layout (location = 6) uniform float OuterRadius = 1;
layout (location = 7) uniform vec4 InnerColor = vec4(1);
layout (location = 8) uniform vec4 MiddleColor = vec4(1);
layout (location = 9) uniform vec4 OuterColor = vec4(1);

void main()
{
	float radius = position.x * position.x + position.y * position.y;
	if(radius < MiddleRadius * MiddleRadius){
		gl_FragColor = InnerColor;
	}
	else {
		gl_FragColor = MiddleColor;
	}
}