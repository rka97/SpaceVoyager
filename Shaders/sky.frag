#version 430 core

in vec4 gl_FragCoord;

layout (location = 6) uniform float iTime = 1.0;
layout (location = 7) uniform vec2 iResolution = vec2(1024, 684);
layout (location = 12) uniform sampler2D texture_diffuse1;

layout (location = 0) out vec4 fragColor;


vec2 fragCoord = vec2(gl_FragCoord.x, gl_FragCoord.y);

const vec3 color1 = vec3(0.0, 0.5,  1.0);
const vec3 color2 = vec3(0.3,  0.7,  1.0);
const vec3 color3 = vec3(0.5,  0.8, 1.0);

const float animationSpeed = 20.0;

float scale = 1.0;

float patternHeight = 80.0;

float sinus1Amplitude = 15.0;
float sinus2Amplitude = 10.0;
float sinus3Amplitude = 10.0;

float sinus1Lambda = 40.0;
float sinus2Lambda = 30.0;
float sinus3Lambda = 33.0;

float sinus1Y = (80.0 / 3.0) * 2.0;
float sinus2Y = (80.0 / 3.0);
float sinus3Y = 0.0;

float sinusYRandomFactor = 0.000002;

float sinus1Movement = -2.0;
float sinus2Movement =  2.3;
float sinus3Movement =  0.2;

float layer1Distance = 2.0;
float layer2Distance = 5.0;
float layer3Distance = 13.0;
float layer4Distance = 25.0;
float layer5Distance = 40.0;


float layerDarkenFactor = 0.1; // good for day night transformations..


/**
 * Drakens the assigned color by the assigned amount.
 */
vec3 darken(vec3 color, float amount) {
    return vec3(color.x - amount, color.y -amount, color.z - amount);
}

float rand(vec2 co){
    float rnd = fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
    if(rnd < 0.35) {
        rnd = 0.35;
    }
    return rnd;
}


float getLayerColorFactor(float distance) {
   float factor = 1.0;
   
   factor -= layerDarkenFactor*smoothstep( -1.0, 1.0, distance-layer2Distance );
   factor -= layerDarkenFactor*smoothstep( -1.0, 1.0, distance-layer3Distance );
   factor -= layerDarkenFactor*smoothstep( -1.0, 1.0, distance-layer4Distance );
   factor -= layerDarkenFactor*smoothstep( -1.0, 1.0, distance-layer5Distance );
   factor = mix( factor, 1.3, 1.0-smoothstep( -1.0, 1.0, distance-layer1Distance ) );

   return factor;
}

vec4 GetWaterColor()
{
    vec3 appliedColor = vec3(0.0, 0.0, 0.0);
    float tick = iTime * animationSpeed;
     
    float patternRelativeY  = mod(fragCoord.y, patternHeight);
    float patternNumber = floor(fragCoord.y /  patternHeight);
    bool isOddpattern = mod(fragCoord.y  / patternHeight, 2.0) > 1.0; 
    
    float rnd =  rand(vec2(patternNumber, patternNumber));
    float rnd0 = rand(vec2(patternNumber - 1.0, patternNumber - 1.0));


    sinus1Movement *= tick * rnd;
    sinus2Movement *= tick * rnd;
    sinus3Movement *= tick;

    
    //sinus1Amplitude *= (rnd + 0.3);
    //sinus2Amplitude *= (rnd + 0.3);
    //sinus3Amplitude *= (rnd + 0.3);
    
    float sinus1 = sin( (fragCoord.x + sinus1Movement) / sinus1Lambda) * sinus1Amplitude;
    float sinus2 = sin( (fragCoord.x + sinus2Movement) / sinus2Lambda) * sinus2Amplitude;
    float sinus3 = sin( (fragCoord.x + sinus3Movement) / sinus3Lambda) * sinus3Amplitude;
    

    // shift to pattern relative position
    sinus1 = sinus1 + sinus1Y + sin(tick / 8.0 * rnd) * 5.0 * rnd;
    sinus2 = sinus2 + sinus2Y + sin(tick / 10.0 * rnd) * 5.0 * rnd;
    sinus3 = sinus3 + sinus3Y;
    
    // expect to be in wave 1...
    appliedColor = color2;
    
    float f = patternRelativeY - sinus3-patternHeight;
    f = smoothstep( -1.0, 1.0, f );
    appliedColor = mix( appliedColor, color3*getLayerColorFactor(sinus3 + patternHeight - patternRelativeY), 1.0-f );
    
    f = patternRelativeY - sinus1;
    f = smoothstep( -1.0, 1.0, f );
    appliedColor = mix( appliedColor, color1*getLayerColorFactor(sinus1 - patternRelativeY), 1.0-f );
    
    f = patternRelativeY - sinus2;
    f = smoothstep( -1.0, 1.0, f );
    appliedColor = mix( appliedColor, color2*getLayerColorFactor(sinus2 - patternRelativeY), 1.0-f );

    f = patternRelativeY - sinus3;
    f = smoothstep( -1.0, 1.0, f );
    appliedColor = mix( appliedColor, color3*getLayerColorFactor(sinus3 - patternRelativeY), 1.0-f );
    
    return vec4( appliedColor, 1.0 );
}


float rand_o (in vec2 uv) { return fract(sin(dot(uv,vec2(12.4124,48.4124)))*48512.41241); }
const vec2 O = vec2(0.,1.);
float noise (in vec2 uv) {
	vec2 b = floor(uv);
	return mix(mix(rand_o(b),rand_o(b+O.yx),.5),mix(rand(b+O),rand_o(b+O.yy),.5),.5);
}


#define LAYERS 8
#define SPEED 20.

void main()
{
	vec2 uv = fragCoord.yx / iResolution.yx;
    
    float stars = 0.;
	float fl, s;
	for (int layer = 0; layer < LAYERS; layer++) {
		fl = float(layer);
		s = (400.-fl*20.);
		stars += step(.1,pow(noise(mod(vec2(uv.x*s + iTime*SPEED - fl*100.,uv.y*s),iResolution.x)),18.)) * (fl/float(LAYERS));
	}
    
	fragColor = GetWaterColor() * vec4( vec3(stars) + vec3(0, 0, 0.5), 1.0 );
}