#version 400

// Input primitive
layout(triangles) in;

// Output primitive and required number of vertices:
// We need two vertices per line segment
layout(line_strip, max_vertices = 22) out;

uniform mat4 ModelViewMatrix;
uniform mat4 Projection;
uniform mat4 MVP;

in vec3 vNormal[];
in vec4 vTangent[];
in vec4 vColor[];

out vec4 Color;

void main()
{
    float normal_length = 0.5f;
    vec4 position[3];
    
    
    vec4 tan_color  = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec4 bit_color  = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    vec4 norm_color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    vec4 wire_color = vec4(0.4f, 0.4f, 0.4f, 1.0f);
    
    // Draw Vectors
    int i;
    for(i=0; i<gl_in.length(); i++)
    {
        vec4 P = gl_in[i].gl_Position;
        vec4 N = vec4(normalize(vNormal[i]), 0.0f);
        vec4 T = vec4(normalize(vTangent[i].xyz), 0.0f);
        vec4 B = vec4(normalize( cross(vNormal[i], vTangent[i].xyz) ) * vTangent[i].w, 0.0f);

        // Normal
        position[i] = MVP * P;      // this point is on the surface of the object so we transform it to camera coordinates with MV
        gl_Position = position[i];
        Color = norm_color;
        EmitVertex();

        // This point in not on the surface, so we must transform it with the normal matrix to avoid the final line segment for the normal
        // to be much smaller than the ones for the T and B
        //gl_Position = MVP * (P + N * normal_length * 30);
        gl_Position = Projection * (ModelViewMatrix * P + vec4(normalize((inverse(inverse(ModelViewMatrix)) * N).xyz), 1.0) * normal_length);
        Color = norm_color;
        EmitVertex();

        EndPrimitive();

        // Tangent
        gl_Position = position[i];
        Color = tan_color;
        EmitVertex();

        gl_Position = Projection * (ModelViewMatrix * P + (normalize(ModelViewMatrix * T)) * normal_length);
        Color = tan_color;
        EmitVertex();

        EndPrimitive();

        // Bitangent
        gl_Position = position[i];
        Color = bit_color;
        EmitVertex();

        gl_Position = Projection * (ModelViewMatrix * P + (normalize(ModelViewMatrix * B)) * normal_length);
        //gl_Position = MVP * (P + B * normal_length);
        Color = bit_color;
        EmitVertex();

        EndPrimitive();
    }
    
    // Draw wireframe
    
    gl_Position = MVP * gl_in[0].gl_Position;
    Color = wire_color;
    EmitVertex();

    gl_Position = MVP * gl_in[1].gl_Position;
    Color = wire_color;
    EmitVertex();

    gl_Position = MVP * gl_in[2].gl_Position;
    Color = wire_color;
    EmitVertex();
   
    gl_Position = MVP * gl_in[0].gl_Position;
    Color = wire_color;
    EmitVertex();
   
    EndPrimitive();
}
