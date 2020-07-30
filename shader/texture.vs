#version 330 core

in vec3 position;
in vec2 textureCoordinates;

out vec3 colour;
out vec2 pass_textureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void){
	gl_Position = projection * view * model * vec4(position,1.0);
	pass_textureCoordinates = textureCoordinates;
	colour = vec3(position.x+0.5, 0.0, position.y+0.5);
}