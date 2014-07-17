#version 420

uniform sampler2D tex_image;

in vec2 texCoords;

out vec4 outputColor;

void main()
{
    vec4 color = texture(tex_image, texCoords);
    
    int channel = 0;
    
    outputColor = vec4(color[channel], color[channel], color[channel], 1.0);
}
