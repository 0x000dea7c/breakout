#version 330 core

out vec4 fragColour;

in vec2 texCoords;

uniform sampler2D image;
uniform vec3 colour;

void main()
{
    fragColour = vec4(colour, 1.f) * texture(image, texCoords);
}
