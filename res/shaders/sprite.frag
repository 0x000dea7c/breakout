#version 330 core

out vec4 fragColour;

in vec2 texCoords;

uniform sampler2D image;
uniform vec3 spriteColour;

void main() {
    fragColour = vec4(spriteColour, 1.f) * texture(image, texCoords);
}
