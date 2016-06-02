#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexture;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec4 VertexTangent;

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

out vec3 LightDir_view;
out vec3 LightDir_tangent;
out vec3 ViewDir_tangent;
out vec2 TexCoord;

out vec3 tangent_model;
out vec3 bitangent_model;
out vec3 normal_geometry_model;

out vec3 tangent_eye;
out vec3 bitangent_eye;
out vec3 normal_geometry_eye;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    tangent_model = VertexTangent.xyz;
    normal_geometry_model = VertexNormal.xyz;
    bitangent_model = normalize( cross(VertexNormal, VertexTangent.xyz) ) * VertexTangent.w;
    
    //Transform normal and tangent to eye space
    normal_geometry_eye = normalize(NormalMatrix * VertexNormal).xyz;
    tangent_eye = normalize(ModelViewMatrix * VertexTangent).xyz;

    // Compute the binormal
    bitangent_eye = normalize(cross(normal_geometry_eye, tangent_eye)) * VertexTangent.w;

    // Matrix for transformation to tangent space (remember that glsl matrices are column-major order)
    mat3 fromEyeToTangentSpace = mat3(tangent_eye.x, bitangent_eye.x, normal_geometry_eye.x,
                                      tangent_eye.y, bitangent_eye.y, normal_geometry_eye.y,
                                      tangent_eye.z, bitangent_eye.z, normal_geometry_eye.z);
                                      

    // The fragment shader needs the light and view vectors in Tangent Space
    vec3 pos_eye = vec3(ModelViewMatrix * vec4(VertexPosition,1.0));

    LightDir_view = normalize(Light.Position.xyz - pos_eye);
    LightDir_tangent = fromEyeToTangentSpace * normalize(Light.Position.xyz - pos_eye);
    ViewDir_tangent = fromEyeToTangentSpace * normalize(-pos_eye);

    TexCoord = VertexTexture;

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
