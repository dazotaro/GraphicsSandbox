#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in vec2 VertexTexture;
layout(location = 3) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec4 LightPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec3 Kd = vec3(0.9f, 0.5f, 0.3f);						// Diffuse reflectivity
vec3 Ld = vec3(0.0f, 1.0f, 1.0f);            			// Diffuse light intensity
mat3 NormalMatrix = mat3(vec3( ModelViewMatrix[0]), vec3( ModelViewMatrix[1]), vec3( ModelViewMatrix[2]));

void main()
{

    vec3 tnorm = normalize( NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    vec3 s = normalize(vec3(LightPosition - eyeCoords));

    LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );

    gl_Position = MVP * vec4(VertexPosition,1.0);
}

