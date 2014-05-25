#version 420

in vec2 TexCoord;

uniform sampler2D ColorTex0;

layout( location = 0 ) out vec4 FragColor;

void main()
{
    vec4 texColor = texture(ColorTex0, TexCoord);

    //FragColor = texColor;
    FragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);
}
