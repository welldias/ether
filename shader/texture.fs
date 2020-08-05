#version 400 core

in vec2 pass_textureCoordinates;
in vec3 surfaceNormal;
in vec3 toLightVector;

out vec4 out_Color;

uniform sampler2D modelTexture;
uniform vec3 lightColour;

void main(void){

	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	
	float nDotl = dot(unitNormal, unitLightVector);
	float brightness = max(nDotl, 0.2);
	vec3 diffuse = brightness * lightColour;
	
	out_Color = vec4(diffuse, 1.0) * texture(modelTexture, pass_textureCoordinates);
	//out_Color = vec4(diffuse, 1.0) * vec4(0.8,0.8,0.8,0.8);
	//out_Color = texture(modelTexture, pass_textureCoordinates);

}
