#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in vec2 VertexTexture;
layout(location = 3) in vec3 VertexNormal;
layout(location = 4) in vec4 VertexTangent;

struct LightInfo
{
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

out vec3 LightDir_tangent;
out vec3 ViewDir_tangent;
out vec2 TexCoord;

out vec3 tangent;
out vec3 bitangent;
out vec3 normal_geometry;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    tangent = VertexTangent.xyz;
    normal_geometry = VertexNormal.xyz;
    bitangent = normalize( cross(VertexNormal, VertexTangent.xyz) ) * VertexTangent.w;
    
    //Transform normal and tangent to eye space
    vec3 norm_eye = normalize(NormalMatrix * VertexNormal);
    vec3 tang_eye = normalize(NormalMatrix * VertexTangent.xyz);

    // Compute the binormal
    vec3 binormal_eye = normalize( cross( norm_eye, tang_eye ) ) * VertexTangent.w;

    // Matrix for transformation to tangent space (remember that glsl matrices are column-major order)
    mat3 fromEyeToTangentSpace = mat3(tang_eye.x, binormal_eye.x, norm_eye.x,
                                      tang_eye.y, binormal_eye.y, norm_eye.y,
                                      tang_eye.z, binormal_eye.z, norm_eye.z);

    // The fragment shader needs the light and view vectors in Tangent Space
    vec3 pos_eye = vec3(ModelViewMatrix * vec4(VertexPosition,1.0));

    LightDir_tangent = fromEyeToTangentSpace * normalize(Light.Position.xyz - pos_eye);
    ViewDir_tangent = fromEyeToTangentSpace * normalize(-pos_eye);

    TexCoord = VertexTexture;

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
