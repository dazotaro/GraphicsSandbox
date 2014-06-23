#version 400

// Input primitive
layout(triangles) in;

// Output primitive and required number of vertices:
// We need two vertices per line segment
layout(line_strip, max_vertices=2) out;

uniform mat4 MVP;

in Vertex
{
    vec3 normal;
    vec4 tangent;
    vec4 color;
} vertex[];

out vec4 Color;

void main()
{
    float normal_length = 0.05f;
    vec4 position[3];

    position[0] = MVP * gl_in[0].gl_Position;
    position[1] = MVP * gl_in[1].gl_Position;
    position[2] = MVP * gl_in[2].gl_Position;

    vec4 v1 = position[1] - position[0];
    vec4 v1 = position[2] - position[0];
    
    vec4 normal = normalize(cross(v1,v2));

    // start point
    vec4 P = (position[0] + position[1] + position[2]) / 3.0f;
    gl_Position = MVP * P;
    Color = vertex.color[0];
    EmitVertex();
    
    // end point
    gl_Position = MVP * (P + normal_length * normal);
    Color = vertex.color[0];
    EmitVertex();
    
    EndPrimitive();
}
