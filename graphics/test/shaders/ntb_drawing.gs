#version 400

// Input primitive
layout(triangles) in;

// Output primitive and required number of vertices:
// We need two vertices per line segment
layout(line_strip, max_vertices=18) out;

uniform mat4 MVP;

in vec3 vNormal[];
in vec4 vTangent[];
in vec4 vColor[];

out vec4 Color;

void main()
{
    float normal_length = 0.05f;
    vec4 position[3];
    
    
    vec4 tan_color  = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec4 bit_color  = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    vec4 norm_color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    
    // Draw Normals
    int i;
    for(i=0; i<gl_in.length(); i++)
    {
        vec4 P = gl_in[i].gl_Position;
        vec4 N = vec4(vNormal[i], 0.0f);
        vec4 T = vec4(vTangent[i].xyz, 0.0f);
        vec4 B = vec4(normalize( cross(vNormal[i], vTangent[i].xyz) ) * vTangent[i].w, 0.0f);

        // Normal
        position[i] = MVP * P;
        gl_Position = position[i];
        Color = norm_color;
        EmitVertex();

        gl_Position = MVP * (P + N * normal_length);
        Color = norm_color;
        EmitVertex();

        EndPrimitive();

        // Tangent
        gl_Position = position[i];
        Color = tan_color;
        EmitVertex();

        gl_Position = MVP * (P + T * normal_length);
        Color = tan_color;
        EmitVertex();

        EndPrimitive();

        // Bitangent
        gl_Position = position[i];
        Color = bit_color;
        EmitVertex();

        gl_Position = MVP * (P + B * normal_length);
        Color = bit_color;
        EmitVertex();

        EndPrimitive();
    }
}
