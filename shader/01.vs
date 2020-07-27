#version 330 core

in vec3 position;
in vec2 textureCoords;

out vec2 pass_textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void){
	gl_Position = view * model * vec4(position,1.0);
	pass_textureCoords = textureCoords;
}