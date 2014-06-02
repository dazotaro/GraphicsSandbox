#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in vec2 VertexTexture;
layout(location = 3) in vec3 VertexNormal;
layout(location = 4) in vec4 VertexTangent;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

out Vertex
{
  vec4 normal;
  vec4 color;
} vertex;

void main()
{
     gl_Position   = vec4(VertexPosition,1.0);
     vertex.normal = vec4(VertexNormal, 1.0f);
     vertex.color  = VertexColor;
}

