#version 430 core

layout (location = 0) in varying vec3 position;

layout (location = 4) uniform float InnerRadius = 0;
layout (location = 5) uniform float MiddleRadius = 0.5;
layout (location = 6) uniform float OuterRadius = 1;
layout (location = 7) uniform vec4 InnerColor = vec4(1);
layout (location = 8) uniform vec4 MiddleColor = vec4(1);
layout (location = 9) uniform vec4 OuterColor = vec4(1);

void main()
{
	float radius = sqrt(position.x * position.x + position.y * position.y);
	vec4 color;
	if (radius >= 1.25 * OuterRadius)
	{
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else if (radius >= 0.65f * OuterRadius && MiddleColor == vec4(0)) /* interpolate color if 0 */
	{
		vec4 slope = (OuterColor - InnerColor) / (OuterRadius - InnerRadius);
		vec4 intercept = InnerColor - slope * InnerRadius;
		color = radius * slope + intercept;
	}
	else
	{
		if(radius < MiddleRadius) {
			color = InnerColor;
		}
		else {
			color = MiddleColor;
		}
	}
	gl_FragColor = color;
}