#version 420

in vec2 TexCoord;

uniform sampler2D tex_image;

out vec4 outputColor;

void main()
{
   outputColor = texture(tex_image, TexCoord);
}
