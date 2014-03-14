#version 400
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in vec2 VertexTexture;
layout(location = 3) in vec3 VertexNormal;
out vec4 Color;

void main()
{
   Color = VertexColor;
   gl_Position = vec4(VertexPosition, 1.0) - vec4(0.0, 0.0, 0.0, 0.0);
}

