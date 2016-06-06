# version 330 core

layout(location = 0) in vec3 position;

out vec4 outPosition;
out vec4 outColor;

uniform mat4 transform;

void main(){
	gl_Position = transform*vec4(position.x, position.y, position.z, 1.0f);

	if(position.y == 0 && position.x == 0){
		outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	} else if(position.x < 0.0 && position.y == 0.0){ //left corner
		outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	} else if(position.x > 0.0 && position.y == 0.0){ //right corner 
		outColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	} else if(position.x == 0.0 && position.y > 0.0){ //up corner
		outColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	} else{
		outColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	outPosition = vec4(position, 1.0f);
}