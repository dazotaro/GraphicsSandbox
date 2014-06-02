#version 400

// Input primitive
layout(triangles) in;

// Output primitive and required number of vertices:
// We need two vertices per line segment
layout(line_strip, max_vertices=12) out;

uniform mat4 MVP;

in Vertex
{
    vec4 normal;
    vec4 color;
} vertex[];

out vec4 Color;

void main()
{
    float normal_length = 0.05f;
    vec4 position[3];
    
    // Draw Normals
    int i;
    for(i=0; i<gl_in.length(); i++)
    {
        vec4 P = gl_in[i].gl_Position;
        vec4 N = vertex[i].normal;

        position[i] = MVP * P;
        gl_Position = position[i];
        Color = vertex[i].color;
        EmitVertex();

        gl_Position = MVP * (P + N * normal_length);
        Color = vertex[i].color;
        EmitVertex();

        EndPrimitive();
    }
    
    // Draw the Wireframe
    gl_Position = position[0];
    Color = vertex[0].color;
    EmitVertex();
    
    gl_Position = position[1];
    Color = vertex[1].color;
    EmitVertex();
    
    EndPrimitive();

    gl_Position = position[1];
    Color = vertex[1].color;
    EmitVertex();
    
    gl_Position = position[2];
    Color = vertex[2].color;
    EmitVertex();
    
    EndPrimitive();

    gl_Position = position[2];
    Color = vertex[2].color;
    EmitVertex();
    
    gl_Position = position[0];
    Color = vertex[0].color;
    EmitVertex();
    
    EndPrimitive();

}
