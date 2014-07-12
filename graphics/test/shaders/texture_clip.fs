#version 400

uniform sampler2D ShadowMap;

in vec2 texCoords;

out vec4 outputColor;

void main()
{
    outputColor = texture(ShadowMap, texCoords);
}
