#version 330 core

in vec2 textureCoords;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    color = mix(texture(texture1, textureCoords), texture(texture2, vec2(textureCoords.x, 1.0 - textureCoords.y)), 0.35);
} 