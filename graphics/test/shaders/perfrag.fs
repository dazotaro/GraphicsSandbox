#version 420

in vec3 Position_eye;
in vec3 Normal_eye;

struct LightInfo {
    vec4 Position; // in eye coordinates
    vec3 Intensity;
};

uniform LightInfo Light;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float shininess;    // Specular shininess factor

layout( location = 0 ) out vec4 FragColor;

vec3 ads(vec4 position, vec3 norm)
{
    vec3 s = normalize( vec3(Light.Position - position) );
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect( -s, norm );
    
    vec3 ambient = Light.Intensity * Ka;
     
    float sDotN = max(dot(s,norm), 0.0);
    vec3 diffuse = Light.Intensity * Kd * sDotN;
    
    float rDotV = dot(r, v);
    vec3 specular = Light.Intensity * Ks * pow(max(rDotV, 0.0), shininess);

    return ambient + diffuse + specular;
}

void main()
{
    FragColor = vec4(ads(vec4(Position_eye, 1.0f), Normal_eye), 1.0);
}
