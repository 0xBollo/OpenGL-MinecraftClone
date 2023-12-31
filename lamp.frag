#version 330 core

in vec2 TexCoords;

out vec4 fragColor;

uniform sampler2D lampTexture;

void main()
{
	fragColor = texture(lampTexture, TexCoords);
}