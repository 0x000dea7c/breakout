#version 330 core

out vec4 fragColour;

in vec2 texCoords;

uniform sampler2D image;

uniform vec2 offsets[9];
uniform float edgeKernel[9];
uniform float blurKernel[9];

uniform bool isChaotic;
uniform bool isShaking;
uniform bool isConfusing;

void main()
{
    fragColour = vec4(0.f);

    vec3 samples[9];

    if(isShaking || isChaotic) {
        for(int i = 0; i < 9; ++i) {
            samples[i] = vec3(texture(image, texCoords + offsets[i]));
        }
    }

    if(isShaking) {
        for(int i = 0; i < 9; ++i) {
            fragColour += vec4(samples[i] * blurKernel[i], 0.f);
        }
        fragColour.a = 1.f;
    } else if(isChaotic) {
        for(int i = 0; i < 9; ++i) {
            fragColour += vec4(samples[i] * edgeKernel[i], 0.f);
        }
        fragColour.a = 1.f;
    } else if(isConfusing) {
        fragColour = vec4(1.f - texture(image, texCoords).rgb, 1.f);
    } else {
        fragColour = texture(image, texCoords);
    }
}
