#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexCoords;

out vec2 texCoords;

uniform mat4 MVP;

void main()
{
   texCoords = VertexTexCoords;

   gl_Position = MVP * vec4(VertexPosition, 1.0);
}

