#version 330 core

out vec4 colour;

in vec2 texCoords;

uniform sampler2D image;

void main()
{
    colour = texture(image, texCoords);
}

