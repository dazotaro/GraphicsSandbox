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
    vec3 I = Light.Intensity;
   
    vec3 ambient = I * Ka;
     
    float sDotN = dot(s,norm);
    vec3 diffuse = I * Kd * max(sDotN, 0.0);
    
    float rDotV = dot(r, v);
    vec3 specular = I * Ks * pow(max(rDotV, 0.0), shininess);

    //return I * (Ka + Kd * max(dot(s, norm), 0.0) + Ks * pow(max(dot(r,v), 0.0), Shininess));
    return ambient + diffuse + specular;
}

void main()
{
    FragColor = vec4(ads(vec4(Position_eye, 1.0f), Normal_eye), 1.0);
}
