#version 330 core

layout(location = 0) in vec4 aVertex; // vec2(position), vec2(texCoords)

out vec2 texCoords;

uniform bool isChaotic;
uniform bool isShaking;
uniform bool isConfusing;
uniform float time;

const float chaoticFactor = .1f;
const float shakingFactor = .05f;

void main()
{
    // @TODO: why are we not using a projection matrix here?
    gl_Position = vec4(aVertex.xy, 0.f, 1.f);

    vec2 textTmp = aVertex.zw;

    if(isChaotic) {
        vec2 pos = vec2(textTmp.x + sin(time) * chaoticFactor,
                        textTmp.y + cos(time) * chaoticFactor);
        texCoords = pos;
    } else if(isConfusing) {
        texCoords = 1.f - textTmp;
    } else {
        texCoords = textTmp;
    }

    if(isShaking) {
        gl_Position.x += sin(time) * shakingFactor;
        gl_Position.y += cos(time) * shakingFactor;
    }
}
