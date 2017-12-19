#version 430 core

in vec4 gl_FragCoord;

layout (location = 6) uniform float iTime = 1.0;
layout (location = 7) uniform vec2 iResolution = vec2(1024, 684);
layout (location = 12) uniform sampler2D texture_diffuse1;

layout (location = 0) out vec4 fragColor;

vec2 fragCoord = vec2(gl_FragCoord.y, gl_FragCoord.x);


float rand (in vec2 uv) { return fract(sin(dot(uv,vec2(12.4124,48.4124)))*48512.41241); }
const vec2 O = vec2(0.,1.);
float noise (in vec2 uv) {
	vec2 b = floor(uv);
	return mix(mix(rand(b),rand(b+O.yx),.5),mix(rand(b+O),rand(b+O.yy),.5),.5);
}

#define LAYERS 8
#define SPEED 20.

void main()
{
	vec2 uv = fragCoord.xy / iResolution.xy;
    
    float stars = 0.;
	float fl, s;
	for (int layer = 0; layer < LAYERS; layer++) {
		fl = float(layer);
		s = (400.-fl*20.);
		stars += step(.1,pow(noise(mod(vec2(uv.x*s + iTime*SPEED - fl*100.,uv.y*s),iResolution.x)),18.)) * (fl/float(LAYERS));
	}
    
	fragColor = vec4( vec3(stars) * (vec3(0.905, 0.415, 0.878)) + vec3(0, 0, 0.3), 1.0 );
}