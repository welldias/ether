#version 400 core

in vec3 pass_Color;
in vec3 surfaceNormal;
in vec3 toLightVector;

out vec4 out_Color;

uniform sampler2D modelTexture;
uniform vec4 lightColour;

void main(void){

	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	
	float nDotl = dot(unitNormal, unitLightVector);
	float brightness = max(nDotl, 0.2);
	vec4 diffuse = brightness * lightColour;
	
	out_Color = diffuse * vec4(pass_Color, 1.0);
	//out_Color = vec4(0.8, 0.8, 0.8, 1.0) * vec4(pass_Color, 1.0);
}
