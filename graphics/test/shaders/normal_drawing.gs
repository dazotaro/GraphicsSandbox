#version 400

layout(triangles) in;

// Three lines will be generated: 6 vertices
layout(line_strip, max_vertices=6) out;

uniform mat4 MVP;

in Vertex
{
  vec4 normal;
  vec4 color;
} vertex[];

out vec4 Color;

void main()
{
  int i;
  float normal_length = 0.05f;
  
  for(i=0; i<gl_in.length(); i++)
  {
    vec3 P = gl_in[i].gl_Position.xyz;
    vec3 N = vertex[i].normal.xyz;
    
    gl_Position = MVP * vec4(P, 1.0);
    Color = vertex[i].color;
    EmitVertex();
    
    gl_Position = MVP * vec4(P + N * normal_length, 1.0);
    Color = vertex[i].color;
    EmitVertex();
    
    EndPrimitive();
  }
}