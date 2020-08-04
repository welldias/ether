#version 330 core

in vec3 position;
in vec2 textureCoordinates;
in vec3 normal;

out vec2 pass_textureCoordinates;
out vec3 surfaceNormal;
out vec3 toLightVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

void main(void){
	
	vec4 worldPosiotn = model * vec4(position,1.0);
	
	gl_Position = projection * view * worldPosiotn;
	
	pass_textureCoordinates = textureCoordinates;
	
	surfaceNormal = (model * vec4(normal , 0.0)).xyz;
	toLightVector = lightPosition - worldPosiotn.xyz;
}