#version 420

in vec3 Position_eye;
in vec3 Normal_eye;

struct LightPositional
{
    vec3 position; // in eye coordinates
    vec3 intensity;
};

uniform int num_pos_lights;
uniform LightPositional light_pos[20];

struct Material
{
	vec3 Kd;            // Diffuse reflectivity
	vec3 Ka;            // Ambient reflectivity
	vec3 Ks;            // Specular reflectivity
	float shininess;    // Specular shininess factor
};

uniform Material material;

layout( location = 0 ) out vec4 FragColor;

vec3 ads(uint index, vec4 position, vec3 norm)
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

void main()
{
	FragColor = vec4(0.0f);
	for (int index = 0; index < num_pos_lights; ++index)
    	FragColor += vec4(ads(index, vec4(Position_eye, 1.0f), Normal_eye), 1.0f / num_pos_lights);
    	
    	//FragColor = vec4(light_pos[0].intensity, 1.0f);
}
