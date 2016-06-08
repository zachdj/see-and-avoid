#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform float attenuation;

void main()
{    
	// simple, distance-based attenuation calculated in aircraft drawer class
    color = vec4(texture(texture_diffuse1, TexCoords)) * attenuation;
}