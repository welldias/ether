#version 330 core


in vec2 pass_textureCoords;

out vec4 out_Color;

uniform sampler2D textureSampler;

void main(void){

	//out_Color = vec4(1.0,1.0,1.0,1.0);
	out_Color = texture(textureSampler, pass_textureCoords);

}
