#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexture;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec4 VertexTangent;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

out vec3 vNormal;
out vec4 vTangent;
out vec4 vColor;

void main()
{
     gl_Position   = vec4(VertexPosition,1.0);
     vNormal = VertexNormal;
     vTangent = VertexTangent;
     vColor  = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

