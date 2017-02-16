#version 420

in vec3 Position_eye;
in vec3 Normal_eye;

struct LightPositional
{
    vec3 position; // in eye coordinates
    vec3 intensity;
};

struct LightSpotlight
{
    vec3 position;  // in eye coordinates
    vec3 direction; // in eye coordinates
    vec3 intensity;
    float cutoff;   // (angle) in radians
};

uniform int num_pos_lights;
uniform LightPositional light_pos[20];
uniform int num_spot_lights;
uniform LightSpotlight light_spot[20];

struct Material
{
	vec3 Kd;            // Diffuse reflectivity
	vec3 Ka;            // Ambient reflectivity
	vec3 Ks;            // Specular reflectivity
	float shininess;    // Specular shininess factor
};

uniform Material material;

layout( location = 0 ) out vec4 FragColor;



vec3 adsPositional(uint index, vec4 position, vec3 norm)
{
    vec3 s = normalize( light_pos[index].position - position.xyz );
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect( -s, norm );
    
    vec3 ambient = light_pos[index].intensity * material.Ka;
     
    float sDotN = max(dot(s,norm), 0.0);
    vec3 diffuse = light_pos[index].intensity * material.Kd * sDotN;
    
    float rDotV = dot(r, v);
    vec3 specular = light_pos[index].intensity * material.Ks * pow(max(rDotV, 0.0), material.shininess);

    //return specular;
    return ambient + diffuse + specular;
    //return glm::vec3(0.2f, 0.2f, 0.2f);
}



vec3 adsSpotlight(uint index, vec4 position, vec3 norm)
{
    vec3 s = normalize( light_spot[index].position - position.xyz );
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect( -s, norm );
    
    vec3 ads = vec3(0.0f);   // Ambient + Diffuse + Specular
    
    // Ambient
    ads += light_spot[index].intensity * material.Ka;
     
    // Discard fragments that fall outside of the spotlight cutoff angle
    if (dot(s, light_spot[index].direction) > cos(light_spot[index].cutoff))
    {
        // Diffuse      
        float sDotN = max(dot(s,norm), 0.0);
        ads += light_spot[index].intensity * material.Kd * sDotN;
        
        // Specular
        float rDotV = dot(r, v);
        ads += light_spot[index].intensity * material.Ks * pow(max(rDotV, 0.0), material.shininess);
    }
        
    return ads;
}

void main()
{
	FragColor = vec4(0.0f);
    
    vec3 normal = normalize(Normal_eye);
    
    // Positional
	for (int index = 0; index < num_pos_lights; ++index)
    	FragColor += vec4(adsPositional(index, vec4(Position_eye, 1.0f), normal), 1.0f);
    	
    // Spotlight
    for (int index = 0; index < num_spot_lights; ++index)
        FragColor += vec4(adsSpotlight(index, vec4(Position_eye, 1.0f), normal), 1.0f);

}
