#version 420

in vec3 Position_eye;
in vec3 Normal_eye;
in vec2 TexCoord;


struct LightInfo
{
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // Light intensity (amb., diff., and spec.)
};

uniform LightInfo Light;

struct Material
{
	vec3 Kd;            // Diffuse reflectivity
	vec3 Ka;            // Ambient reflectivity
	vec3 Ks;            // Specular reflectivity
	float shininess;    // Specular shininess factor
};

uniform Material material;
uniform sampler2D ColorTex0;

layout( location = 0 ) out vec4 FragColor;

vec3 ads(vec4 position, vec3 norm)
{
	vec4 Kad = texture(ColorTex0, TexCoord);
    vec3 s = normalize( vec3(Light.Position - position) );
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect( -s, norm );
    vec3 I = Light.Intensity;
   
    vec3 ambient = I * Kad.rgb;
     
    float sDotN = dot(s,norm);
    vec3 diffuse = I * Kad.rgb * max(sDotN, 0.0);
    
    float rDotV = dot(r, v);
    //vec3 specular = I * material.Ks * pow(max(rDotV, 0.0), material.shininess);
    vec3 specular = I * Kad.rgb * pow(max(rDotV, 0.0), material.shininess);

    //return I * (Ka + Kd * max(dot(s, norm), 0.0) + Ks * pow(max(dot(r,v), 0.0), Shininess));
    return ambient + diffuse + specular;
}

void main()
{
    FragColor = vec4(ads(vec4(Position_eye, 1.0f), normalize(Normal_eye)), 1.0);
}
