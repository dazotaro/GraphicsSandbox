#version 400

// Input primitive
layout(triangles) in;

// Output primitive and required number of vertices:
// We need two vertices per line segment
layout(line_strip, max_vertices=6) out;

uniform mat4 MVP;

in vec4 color[];

out vec4 Color;

void main()
{
    // Draw the Wireframe
    gl_Position = MVP * gl_in[0].gl_Position;
    Color = color[0];
    EmitVertex();
    
    gl_Position = MVP * gl_in[1].gl_Position;
    Color = color[1];
    EmitVertex();
    
    EndPrimitive();

    gl_Position = MVP * gl_in[1].gl_Position;
    Color = color[1];
    EmitVertex();
    
    gl_Position = MVP * gl_in[2].gl_Position;
    Color = color[2];
    EmitVertex();
    
    EndPrimitive();

    gl_Position = MVP * gl_in[2].gl_Position;
    Color = color[2];
    EmitVertex();
    
    gl_Position = MVP * gl_in[0].gl_Position;
    Color = color[0];
    EmitVertex();
    
    EndPrimitive();
}
