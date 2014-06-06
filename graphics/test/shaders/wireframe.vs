#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexture;
layout(location = 2) in vec3 VertexNormal;

out vec4 color;

void main()
{
     gl_Position = vec4(VertexPosition,1.0);
     color       = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

