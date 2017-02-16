#version 420

in vec3 Position;
in vec3 Normal;

struct LightInfo
{
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // Light intensity (amb., diff., and spec.)
};

uniform LightInfo lights[8];

uniform vec4 LightPosition;
uniform vec3 LightIntensity;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

layout( location = 0 ) out vec4 FragColor;

vec3 ads(int lightIndex, vec4 position, vec3 norm)
{
    vec3 s = normalize( vec3(lights[lightIndex].Position - position) );
    vec3 v = normalize(vec3(-position));
    vec3 h = normalize(v + s);
    vec3 I = lights[lightIndex].Intensity;

    return I * (Ka + Kd * max(dot(s, norm), 0.0) + Ks * pow(max(dot(h, norm), 0.0), Shininess));
}

void main()
{
    vec3 color = vec3(0.0f);
    vec3 normal = normalize(Normal);
    // Evaluate the lighting equation, for each light
    for( int i = 0; i < 8; i++ )
        color += ads( i, vec4(Position, 1.0f), normal);

    FragColor = vec4(color, 1.0);
}
