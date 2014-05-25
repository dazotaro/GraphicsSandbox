#version 420

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;


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

uniform sampler2D ColorTex0;

layout( location = 0 ) out vec4 FragColor;

vec3 ads(vec4 position, vec3 normal, out vec3 ambAndDiff, out vec3 spec)
{
    vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect(-s, normal);
    vec3 I = Light.Intensity;

	ambAndDiff = I * (Ka + Kd * max(dot(s, normal), 0.0));
	spec = I * (Ks * pow(max(dot(r,v), 0.0), Shininess));
	
	return ambAndDiff + spec;
}

void main()
{
	vec3 totalAmbDiff = vec3(0.0);
	vec3 totalSpec    = vec3(0.0);
	vec4 texColor = texture(ColorTex0, TexCoord);

	vec3 AaD, spec;
    ads(vec4(Position, 1.0f), Normal, AaD, spec);
	
    FragColor = vec4(AaD, 1.0) * texColor + vec4(spec, 1.0);
}
