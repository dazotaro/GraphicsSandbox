#version 420

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;


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

uniform sampler2D Tex1;

layout( location = 0 ) out vec4 FragColor;

vec3 ads(int lightIndex, vec4 position, vec3 norm, out vec3 ambAndDiff, out vec3 spec)
{
    vec3 s = normalize( vec3(lights[lightIndex].Position - position) );
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect( -s, norm );
    vec3 I = lights[lightIndex].Intensity;

	ambAndDiff = I * (Ka + Kd * max(dot(s, norm), 0.0));
	spec = I * (Ks * pow(max(dot(r,v), 0.0), Shininess));
	
	return ambAndDiff + spec;
}

void main()
{
	vec3 totalAmbDiff = vec3(0.0);
	vec3 totalSpec    = vec3(0.0);
	vec4 texColor = texture(Tex1, TexCoord);

    // Evaluate the lighting equation, for each light
    for (int i = 0; i < 8; i++)
    {
    	vec3 AaD, spec;
        ads(i, vec4(Position, 1.0f), Normal, AaD, spec);
        totalAmbDiff += AaD;
        totalSpec    += spec;
	}
	
    FragColor = vec4(totalAmbDiff, 1.0) * texColor + vec4(totalSpec, 1.0);
}
