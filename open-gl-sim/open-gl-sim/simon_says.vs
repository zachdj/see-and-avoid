# version 330 core

layout(location = 0) in vec3 position;

out vec4 outPosition;

uniform float xOffset;
uniform float yOffset;

void main(){
	gl_Position = vec4(position.x + xOffset, position.y + yOffset, position.z, 1.0f);

	outPosition = vec4(position, 1.0f);
}