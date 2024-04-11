#version 330 core

layout(location = 0) in vec4 aVertex; // vec2(position), vec2(texCoords)

out vec4 aColour;
out vec2 aTexCoords;

uniform mat4 projection;
uniform vec4 colour;
uniform vec2 offset;

const float scale = 20.f;

void main()
{
    aColour = colour;
    aTexCoords = aVertex.zw;

    gl_Position = projection * vec4((aVertex.xy) * scale + offset, 0.f, 1.f);
}
