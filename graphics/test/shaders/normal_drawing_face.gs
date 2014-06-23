#version 400

// Input primitive
layout(triangles) in;

// Output primitive and required number of vertices:
// We need two vertices per line segment
layout(line_strip, max_vertices=2) out;

uniform mat4 MVP;

in vec3 vNormal[];
in vec4 vTangent[];
in vec4 vColor[];

out vec4 Color;

void main()
{
    float normal_length = 0.02f;
    vec3 position[3];

    position[0] = gl_in[0].gl_Position.xyz;
    position[1] = gl_in[1].gl_Position.xyz;
    position[2] = gl_in[2].gl_Position.xyz;

    vec3 v1 = (position[1] - position[0]);
    vec3 v2 = (position[2] - position[0]);
    
    vec3 normal = normalize(cross(v1,v2));

    // start point
    vec4 P = vec4((position[0] + position[1] + position[2]) / 3.0f, 1.0f);
    gl_Position = MVP * P;
    Color = vColor[0];
    EmitVertex();
    
    // end point
    gl_Position = MVP * vec4(P.xyz + normal_length * normal, 1.0f);
    Color = vColor[0];
    EmitVertex();
    
    EndPrimitive();
}
