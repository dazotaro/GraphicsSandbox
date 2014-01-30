#version 420

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in vec2 VertexTexture;
layout(location = 3) in vec3 VertexNormal;

out vec2 TexCoord;

uniform mat4 MVP;

void main()
{
    TexCoord = VertexTexture;
    
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
