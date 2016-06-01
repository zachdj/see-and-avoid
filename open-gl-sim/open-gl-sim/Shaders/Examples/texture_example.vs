# version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoords;

out vec4 ourColor;
out vec2 textureCoords; //just gonna pass these through to the frag shader

void main(){
	gl_Position = vec4(position, 1.0f);

	ourColor = vec4(color, 1.0f);
	textureCoords = vec2(texCoords);
}