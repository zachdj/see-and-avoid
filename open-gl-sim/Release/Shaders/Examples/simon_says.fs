#version 330 core

in vec4 outPosition;

out vec4 color;

void main()
{
	if(outPosition.x <= 0.0 && outPosition.y >= 0.0){
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	} else if(outPosition.x <= 0.0 && outPosition.y < 0.0){
		color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	} else if(outPosition.x > 0.0 && outPosition.y >= 0.0){
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	} else{
		color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
} 