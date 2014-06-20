#version 400

struct LightInfo {
    vec4 Position;
    vec3 Intensity;
};

uniform LightInfo Light;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float shininess;    // Specular shininess factor

uniform sampler2DShadow ShadowMap;

in vec3 Position;
in vec3 Normal;
in vec4 ShadowCoord;

layout (location = 0) out vec4 FragColor;

vec3 phongModelDiffAndSpec()
{
    vec3 n = Normal;
    //if( !gl_FrontFacing ) n = -n;
    vec3 s = normalize(vec3(Light.Position) - Position);
    vec3 v = normalize(-Position.xyz);
    vec3 r = reflect( -s, n );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = Light.Intensity * Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Ks *
            pow( max( dot(r,v), 0.0 ), shininess );

    return diffuse + spec;
}

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

subroutine (RenderPassType)
void shadeWithShadow()
{
    vec3 ambient = Light.Intensity * Ka;
    vec3 diffAndSpec = phongModelDiffAndSpec();

    float shadow = textureProj(ShadowMap, ShadowCoord);
    //float shadow = 1.0;

    // If the fragment is in shadow, use ambient light only.
    FragColor = vec4(diffAndSpec * shadow + ambient, 1.0);

    // Gamma correct
    FragColor = pow( FragColor, vec4(1.0 / 2.2) );
}

subroutine (RenderPassType)
void recordDepth()
{
    // Do nothing, depth will be written automatically
}

void main() {
    // This will call either shadeWithShadow or recordDepth
    RenderPass();
}
