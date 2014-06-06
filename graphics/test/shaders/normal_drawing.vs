#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexture;
layout(location = 2) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

out Vertex
{
  vec4 normal;
  vec4 color;
} vertex;

void main()
{
     gl_Position   = vec4(VertexPosition,1.0);
     vertex.normal = vec4(VertexNormal, 0.0f);
     vertex.color  = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

