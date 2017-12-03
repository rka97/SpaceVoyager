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
	vec3 Ks;
	float Shininess;
};

layout (location = 12) uniform sampler2D texture_diffuse1;
layout (location = 13) uniform LightInfo light; // will take 13 and 14.
layout (location = 15) uniform MaterialInfo material; // will take 15-18.

layout (location = 0) out vec4 out_color;

const int diffuseLevels = 4;
const float diffuseScaleFactor = 1.0 / diffuseLevels;
const int specularLevels = 2;
const float specularScaleFactor = 1.0 / specularLevels;

vec3 ToonShade()
{
	vec3 s = normalize (light.position.xyz - position.xyz);
	float cosineSN = max ( 0.0, dot (s, normal) );
	vec3 diffuse = material.Kd * floor (cosineSN * diffuseLevels) * diffuseScaleFactor;
	vec3 v = normalize (vec3(-position));
	vec3 r = reflect (-s, normal);
	float cosineRV = max (0.0, dot (r, v));
	vec3 specular = material.Ks * pow ( floor(cosineRV * specularLevels) * specularScaleFactor, material.Shininess );

	return light.intensity * (material.Ka + diffuse + specular * 0.25f);
}

float threshold(in float thr1, in float thr2 , in float val) {
 if (val < thr1) {return 0.0;}
 if (val > thr2) {return 1.0;}
 return val;
}

// averaged pixel intensity from 3 color channels
float avg_intensity(in vec4 pix) {
 return (pix.r + pix.g + pix.b)/3.;
}

vec4 get_pixel(in vec2 coords, in float dx, in float dy) {
 return texture(texture_diffuse1, texCoordinates + vec2(dx, dy));
}

// returns pixel color
float IsEdge(in vec2 coords){
  float dxtex = 1.0 / 512.0 /*image width*/;
  float dytex = 1.0 / 512.0 /*image height*/;
  float pix[9];
  int k = -1;
  float delta;

  // read neighboring pixel intensities
  for (int i=-1; i<2; i++) {
   for(int j=-1; j<2; j++) {
    k++;
    pix[k] = avg_intensity(get_pixel(coords,float(i)*dxtex,
                                          float(j)*dytex));
   }
  }

  // average color differences around neighboring pixels
  delta = (abs(pix[1]-pix[7])+
          abs(pix[5]-pix[3]) +
          abs(pix[0]-pix[8])+
          abs(pix[2]-pix[6])
           )/4.;

  return threshold(0.15,0.4,clamp(1.8*delta,0.0,1.0));
}

void main()
{
	vec4 texColor = texture(texture_diffuse1, texCoordinates); 
	/* For a retro style: enable this. colorLevels^3 colors only! */
	/*
	const int colorLevels = 16;
	const float colorScaleFactor = 1.0 / colorLevels;
	texColor.x = floor(texColor.x * colorLevels) * colorScaleFactor;
	texColor.y = floor(texColor.y * colorLevels) * colorScaleFactor;
	texColor.z = floor(texColor.z * colorLevels) * colorScaleFactor;
	*/
	vec4 toonShadingColor = vec4(ToonShade(), 1.0);
	float val_isEdge = 1 - IsEdge(texCoordinates);
	vec4 edge_clr = vec4(val_isEdge, val_isEdge, val_isEdge, 1.0);
	if (val_isEdge < 0.1)
		out_color = edge_clr;
	else
		out_color = toonShadingColor * texColor;
}