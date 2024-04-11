#version 330 core

out vec4 fragColour;

in vec2 texCoords;

uniform sampler2D image;

void main() {
    fragColour = texture(image, texCoords);
}
