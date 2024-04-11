#version 330 core

out vec4 fragColour;

in vec4 aColour;
in vec2 aTexCoords;

uniform sampler2D image;

void main()
{
    fragColour = texture(image, aTexCoords) * aColour;
}
