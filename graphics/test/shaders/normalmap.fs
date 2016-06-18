#version 400

in vec3 LightDir_tangent;
in vec3 ViewDir_tangent;
in vec2 TexCoord;

uniform sampler2D ColorTex0;
uniform sampler2D NormalMapTex;

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

layout( location = 0 ) out vec4 FragColor;

vec3 phongModel(vec3 norm, vec3 diffR)
{
    vec3 lightVec_tangent = normalize(LightDir_tangent);
    vec3 eyeVec_tangent = normalize(ViewDir_tangent);
    vec3 ambient = Light.Intensity * 0.2f * diffR;
    vec3 spec_mat = vec3(1.0f, 1.0f, 1.0f);
    float shininess = 100.0f;
    
    float sDotN = clamp(dot(lightVec_tangent, norm), 0, 1);
    vec3 diffuse = Light.Intensity * diffR * sDotN;

    vec3 spec = vec3(0.0);
    float rDotV = 0.0;
    if (sDotN > 0.0)
    {
        vec3 r = reflect(-lightVec_tangent, norm );
        rDotV = max(dot(r, eyeVec_tangent), 0.0);
        spec = Light.Intensity * spec_mat * pow(rDotV, shininess);
    }

    return ambient + diffuse + spec;
}

void main()
{
    // Lookup the normal from the normal map
    // (it could also get loaded with a GL_RGB_SNORM format in glTexImage2D to avoid the avoid the * 2.0 - 1.0 calculation)
	vec3 normal = normalize(texture(NormalMapTex, TexCoord).rgb * 2.0 - 1.0);

	vec4 texColor = texture(ColorTex0, TexCoord);

    FragColor = vec4(phongModel(normal, texColor.rgb), 1.0);
}
