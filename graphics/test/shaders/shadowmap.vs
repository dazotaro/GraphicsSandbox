#version 400

// GPU gems 3 Chapter 10
// GPU gems 2 Chapter 17
// GPU gems Chapter ??
// NVIDIA Whitepaper

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexture;
layout(location = 2) in vec3 VertexNormal;
//layout(location = 3) in vec4 VertexTangent;

out vec3 Position;
out vec3 Normal;
out vec4 ShadowCoord;

uniform mat4 Model;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform mat4 ShadowMatrix;

void main()
{
    Position = (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;
    Normal = normalize( NormalMatrix * VertexNormal );
    mat4 model2light = ShadowMatrix * Model;
    ShadowCoord = model2light * vec4(VertexPosition,1.0);
    gl_Position = MVP * vec4(VertexPosition,1.0);
}
