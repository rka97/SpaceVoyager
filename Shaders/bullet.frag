#version 430 core

layout (location = 0) in varying vec3 position;

layout (location = 4) uniform float InnerRadius = 0;
layout (location = 5) uniform float MiddleRadius = 0.5;
layout (location = 6) uniform float OuterRadius = 1;
layout (location = 7) uniform vec4 InnerColor = vec4(1);
layout (location = 8) uniform vec4 MiddleColor = vec4(1);
layout (location = 9) uniform vec4 OuterColor = vec4(1);

layout (location = 0) out vec4 out_color;

void main()
{
	float radius = sqrt(position.x * position.x + position.y * position.y);
	vec4 color;
	if (radius >= 1.25 * OuterRadius)
	{
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else if (vec3(MiddleColor) == vec3(0, 0, 0) && radius >= 0.2 * OuterRadius) /* interpolate color if 0 */
	{
		if (radius >= MiddleColor.w * OuterRadius)
		{
			vec4 slope = (OuterColor - InnerColor) / (OuterRadius - InnerRadius);
			vec4 intercept = InnerColor - slope * InnerRadius;
			color = radius * slope + intercept;
		}
		else
		{
			color = InnerColor;
		}
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
	out_color = color;
}