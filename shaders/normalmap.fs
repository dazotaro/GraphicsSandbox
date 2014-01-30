#version 400

in vec3 LightDir_tangent;
in vec3 ViewDir_tangent;
in vec2 TexCoord;

in vec3 tangent;
in vec3 bitangent;
in vec3 normal_geometry;

uniform sampler2D ColorTex0;
uniform sampler2D NormalMapTex;

struct LightInfo
{
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // Light intensity (amb., diff., and spec.)
};

uniform LightInfo Light;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

layout( location = 0 ) out vec4 FragColor;

vec3 phongModel(vec3 norm, vec3 diffR)
{
    vec3 ambient = Light.Intensity * Ka * diffR;

    float sDotN = max( dot(LightDir_tangent, norm), 0.0 );
    vec3 diffuse = Light.Intensity * diffR * sDotN;

    vec3 spec = vec3(0.0);
    float rDotV = 0.0;
    if (sDotN > 0.0)
    {
        vec3 r = reflect(-LightDir_tangent, norm );
        rDotV = max(dot(r, ViewDir_tangent), 0.0);
        spec = Light.Intensity * Ks * pow(max(dot(r,ViewDir_tangent), 0.0), Shininess);
    }

    return ambient + diffuse;// + spec;
}

void main()
{
    // Lookup the normal from the normal map
	vec4 normal = normalize(texture(NormalMapTex, TexCoord) * 2.0 - 1.0);
    /*vec4 normal = texture(NormalMapTex, TexCoord);
    normal.x -= 0.5;
    normal.y -= 0.5;
    normal = normalize(normal);*/
	//vec4 normal = texture(NormalMapTex, TexCoord);

	vec4 texColor = texture(ColorTex0, TexCoord);

    FragColor = vec4(phongModel(normal.xyz, texColor.rgb), 1.0);
}
