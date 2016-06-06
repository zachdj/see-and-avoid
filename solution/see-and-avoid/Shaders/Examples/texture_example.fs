#version 330 core

in vec4 ourColor;
in vec2 textureCoords;

out vec4 color;

uniform sampler2D boxTexture;
uniform sampler2D acmeTexture;

void main()
{
    color = mix(texture(boxTexture, textureCoords), texture(acmeTexture, vec2(textureCoords.x, 1.0 - textureCoords.y)), 0.35);
} 