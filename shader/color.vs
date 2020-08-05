#version 400 core

in vec3 position;
in vec3 color;
in vec3 normal;

out vec3 pass_Color;
out vec3 surfaceNormal;
out vec3 toLightVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

void main(void){
	
	vec4 worldPosition = model * vec4(position,1.0);
	
	gl_Position = projection * view * worldPosition;
	
	pass_Color = color;
	
	surfaceNormal = (model * vec4(normal , 0.0)).xyz;
	toLightVector = lightPosition - worldPosition.xyz;
}