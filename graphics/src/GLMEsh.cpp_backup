/*
 * GLMesh.cpp
 *
 *  Created on: May 7, 2013
 *      Author: jusabiaga
 */

#include "GLMesh.hpp"
#include <iostream>     // std::cout, std::endl
#include "DebugGlm.hpp" // overloaded 'operator<<' for GLM classes
/**
* @brief Non-Default Constructor
*
* @param mesh Mesh object containing the data for this object
*/
GLMesh::GLMesh(const Mesh &mesh) : Mesh(mesh)
{
    vao_handle_  = 0;
    vbo_handles_ = 0;	// NULL
}

/**
* @brief Non-Default Constructor
*
* @param name Id of the Mesh
* @param positions Vector with all vertex positions
* @param normals Vector with all vertex normals
* @param colors Vector with all vertex colors
* @param tex_coords Vector with all vertex colors
* @param faces Vector with all info about the faces of the Mesh
*/
GLMesh::GLMesh(const std::string  &name,
               const PositionList &positions,
               const NormalList   &normals,
               const TangentList  &tangents,
               const ColorList    &colors,
               const TexCoordList &tex_coords,
               const FaceList     &faces)
		: Mesh(name, positions, normals, tangents, colors, tex_coords, faces), vao_handle_(0), vbo_handles_(0)
{
    vao_handle_ = 0;
}

/**
* @brief Destructor
*/
GLMesh::~GLMesh()
{
	// Delete the buffers
    glDeleteBuffers(5, vbo_handles_);
    // Delete the vertex array
    glDeleteVertexArrays(1, &vao_handle_);
    // Release the handles
    delete [] vbo_handles_;
}

/**
* @brief Create Vertex Buffer Object
*
* @detail If the data is not yet in a VBO, create and update the handle to it
*
* @return Successful?
*
* \todo Avoid duplicity of data by not duplicating vertices
* \todo Warning, this assumes each face is a triangle
*/
bool GLMesh::init(void)
{
    return initVBOs();
}


/**
* @brief Create Vertex Buffer Object
*
* @detail If the data is not yet in a VBO, create and update the handle to it
*
* @return Successful?
*
* \todo Avoid duplicity of data by not duplicating vertices
* \todo Warning, this assumes each face is a triangle
*/
bool GLMesh::initVBOs(void)
{
    // Retrieve the data from the Mesh object
    const Mesh::FaceList     &faces      = getFaces();
    const Mesh::PositionList &positions  = getPositions();
    const Mesh::NormalList   &normals    = getNormals();
    const Mesh::TangentList  &tangents   = getTangents();
    const Mesh::ColorList    &colors     = getColors();
    const Mesh::TexCoordList &tex_coords = getTexCoords();

    const unsigned int num_faces = faces.size();

    // WARNING! Assuming that each face is a triangle, we have three vertices per face
    const unsigned int num_vertex_data = 3 * num_faces;

    float *positions_per_vertex    = new float [num_vertex_data * Mesh::POSITION_VECTOR_SIZE];
    float *normals_per_vertex      = new float [num_vertex_data * Mesh::NORMAL_VECTOR_SIZE];
    float *tangents_per_vertex     = new float [num_vertex_data * Mesh::TANGENT_VECTOR_SIZE];
    float *colors_per_vertex       = new float [num_vertex_data * Mesh::COLOR_VECTOR_SIZE];
    float *tex_coords_per_vertex   = new float [num_vertex_data * Mesh::TEX_COORDS_VECTOR_SIZE];

    //
    for (unsigned int index = 0; index < num_faces; ++index)
    {
        Mesh::IndexList index_list;

        // Offsets to the three vertices of the face

        const unsigned int vertex_offset[3] = {(index*3) + 0, (index*3) + 1, (index*3) + 2};

        // Filled the arrays
        // Positions
        faces[index].getIndices(Mesh::Face::VERTEX_INDEX, index_list);
        assert(index_list.size() == 3);                                     // Is this a triangle?
        positions_per_vertex[vertex_offset[0]*Mesh::POSITION_VECTOR_SIZE + 0] = positions[index_list[0]].x;
        positions_per_vertex[vertex_offset[0]*Mesh::POSITION_VECTOR_SIZE + 1] = positions[index_list[0]].y;
        positions_per_vertex[vertex_offset[0]*Mesh::POSITION_VECTOR_SIZE + 2] = positions[index_list[0]].z;
        positions_per_vertex[vertex_offset[1]*Mesh::POSITION_VECTOR_SIZE + 0] = positions[index_list[1]].x;
        positions_per_vertex[vertex_offset[1]*Mesh::POSITION_VECTOR_SIZE + 1] = positions[index_list[1]].y;
        positions_per_vertex[vertex_offset[1]*Mesh::POSITION_VECTOR_SIZE + 2] = positions[index_list[1]].z;
        positions_per_vertex[vertex_offset[2]*Mesh::POSITION_VECTOR_SIZE + 0] = positions[index_list[2]].x;
        positions_per_vertex[vertex_offset[2]*Mesh::POSITION_VECTOR_SIZE + 1] = positions[index_list[2]].y;
        positions_per_vertex[vertex_offset[2]*Mesh::POSITION_VECTOR_SIZE + 2] = positions[index_list[2]].z;

        // Normals
        normals_per_vertex[vertex_offset[0]*Mesh::NORMAL_VECTOR_SIZE + 0] = normals[index_list[0]].x;
        normals_per_vertex[vertex_offset[0]*Mesh::NORMAL_VECTOR_SIZE + 1] = normals[index_list[0]].y;
        normals_per_vertex[vertex_offset[0]*Mesh::NORMAL_VECTOR_SIZE + 2] = normals[index_list[0]].z;
        normals_per_vertex[vertex_offset[1]*Mesh::NORMAL_VECTOR_SIZE + 0] = normals[index_list[1]].x;
        normals_per_vertex[vertex_offset[1]*Mesh::NORMAL_VECTOR_SIZE + 1] = normals[index_list[1]].y;
        normals_per_vertex[vertex_offset[1]*Mesh::NORMAL_VECTOR_SIZE + 2] = normals[index_list[1]].z;
        normals_per_vertex[vertex_offset[2]*Mesh::NORMAL_VECTOR_SIZE + 0] = normals[index_list[2]].x;
        normals_per_vertex[vertex_offset[2]*Mesh::NORMAL_VECTOR_SIZE + 1] = normals[index_list[2]].y;
        normals_per_vertex[vertex_offset[2]*Mesh::NORMAL_VECTOR_SIZE + 2] = normals[index_list[2]].z;

        // Normals
        tangents_per_vertex[vertex_offset[0]*Mesh::TANGENT_VECTOR_SIZE + 0] = tangents[index_list[0]].x;
        tangents_per_vertex[vertex_offset[0]*Mesh::TANGENT_VECTOR_SIZE + 1] = tangents[index_list[0]].y;
        tangents_per_vertex[vertex_offset[0]*Mesh::TANGENT_VECTOR_SIZE + 2] = tangents[index_list[0]].z;
        tangents_per_vertex[vertex_offset[1]*Mesh::TANGENT_VECTOR_SIZE + 0] = tangents[index_list[1]].x;
        tangents_per_vertex[vertex_offset[1]*Mesh::TANGENT_VECTOR_SIZE + 1] = tangents[index_list[1]].y;
        tangents_per_vertex[vertex_offset[1]*Mesh::TANGENT_VECTOR_SIZE + 2] = tangents[index_list[1]].z;
        tangents_per_vertex[vertex_offset[2]*Mesh::TANGENT_VECTOR_SIZE + 0] = tangents[index_list[2]].x;
        tangents_per_vertex[vertex_offset[2]*Mesh::TANGENT_VECTOR_SIZE + 1] = tangents[index_list[2]].y;
        tangents_per_vertex[vertex_offset[2]*Mesh::TANGENT_VECTOR_SIZE + 2] = tangents[index_list[2]].z;

        // Colors
        faces[index].getIndices(Mesh::Face::COLOR_INDEX, index_list);
        assert(index_list.size() == 3);                                     // Is this a triangle?
        colors_per_vertex[vertex_offset[0]*Mesh::COLOR_VECTOR_SIZE + 0] = colors[index_list[0]].r;
        colors_per_vertex[vertex_offset[0]*Mesh::COLOR_VECTOR_SIZE + 1] = colors[index_list[0]].g;
        colors_per_vertex[vertex_offset[0]*Mesh::COLOR_VECTOR_SIZE + 2] = colors[index_list[0]].b;
        colors_per_vertex[vertex_offset[0]*Mesh::COLOR_VECTOR_SIZE + 3] = colors[index_list[0]].a;
        colors_per_vertex[vertex_offset[1]*Mesh::COLOR_VECTOR_SIZE + 0] = colors[index_list[1]].r;
        colors_per_vertex[vertex_offset[1]*Mesh::COLOR_VECTOR_SIZE + 1] = colors[index_list[1]].g;
        colors_per_vertex[vertex_offset[1]*Mesh::COLOR_VECTOR_SIZE + 2] = colors[index_list[1]].b;
        colors_per_vertex[vertex_offset[1]*Mesh::COLOR_VECTOR_SIZE + 3] = colors[index_list[1]].a;
        colors_per_vertex[vertex_offset[2]*Mesh::COLOR_VECTOR_SIZE + 0] = colors[index_list[2]].r;
        colors_per_vertex[vertex_offset[2]*Mesh::COLOR_VECTOR_SIZE + 1] = colors[index_list[2]].g;
        colors_per_vertex[vertex_offset[2]*Mesh::COLOR_VECTOR_SIZE + 2] = colors[index_list[2]].b;
        colors_per_vertex[vertex_offset[2]*Mesh::COLOR_VECTOR_SIZE + 3] = colors[index_list[2]].a;

        // Texture Coordinates
        faces[index].getIndices(Mesh::Face::TEXTURE_INDEX, index_list);
        assert(index_list.size() == 3);                                     // Is this a triangle?
        tex_coords_per_vertex[vertex_offset[0]*Mesh::TEX_COORDS_VECTOR_SIZE + 0] = tex_coords[index_list[0]].s;
        tex_coords_per_vertex[vertex_offset[0]*Mesh::TEX_COORDS_VECTOR_SIZE + 1] = tex_coords[index_list[0]].t;
        tex_coords_per_vertex[vertex_offset[1]*Mesh::TEX_COORDS_VECTOR_SIZE + 0] = tex_coords[index_list[1]].s;
        tex_coords_per_vertex[vertex_offset[1]*Mesh::TEX_COORDS_VECTOR_SIZE + 1] = tex_coords[index_list[1]].t;
        tex_coords_per_vertex[vertex_offset[2]*Mesh::TEX_COORDS_VECTOR_SIZE + 0] = tex_coords[index_list[2]].s;
        tex_coords_per_vertex[vertex_offset[2]*Mesh::TEX_COORDS_VECTOR_SIZE + 1] = tex_coords[index_list[2]].t;
    }

    // Create and bind VAO
    glGenVertexArrays(1, &vao_handle_);
    glBindVertexArray(vao_handle_);

    // Create Buffers
    vbo_handles_ = new GLuint[5];
    glGenBuffers(5, vbo_handles_);

    // Position VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[0]);
    glBufferData(GL_ARRAY_BUFFER, num_vertex_data * Mesh::POSITION_VECTOR_SIZE * sizeof(positions_per_vertex[0]), &positions_per_vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, Mesh::POSITION_VECTOR_SIZE, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(0);   // Vertex positions

    // Color VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[1]);
    glBufferData(GL_ARRAY_BUFFER, num_vertex_data * Mesh::COLOR_VECTOR_SIZE * sizeof(colors_per_vertex[0]), &colors_per_vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, Mesh::COLOR_VECTOR_SIZE, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(1);   // Vertex colors

    // Texture Coordinates VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[2]);
    glBufferData(GL_ARRAY_BUFFER, num_vertex_data * Mesh::TEX_COORDS_VECTOR_SIZE * sizeof(tex_coords_per_vertex[0]), &tex_coords_per_vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, Mesh::TEX_COORDS_VECTOR_SIZE, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(2);   // Vertex texture coordinates

    // Normal VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[3]);
    glBufferData(GL_ARRAY_BUFFER, num_vertex_data * Mesh::NORMAL_VECTOR_SIZE * sizeof(normals_per_vertex[0]), &normals_per_vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, Mesh::NORMAL_VECTOR_SIZE, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(3);   // Vertex normals

    // Tangent VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[4]);
    glBufferData(GL_ARRAY_BUFFER, num_vertex_data * Mesh::TANGENT_VECTOR_SIZE * sizeof(tangents_per_vertex[0]), &tangents_per_vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(4, Mesh::TANGENT_VECTOR_SIZE, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    glEnableVertexAttribArray(4);   // Vertex tangents

    // Clean up
    delete [] positions_per_vertex;
    delete [] normals_per_vertex;
    delete [] tangents_per_vertex;
    delete [] colors_per_vertex;
    delete [] tex_coords_per_vertex;

    return true;
}



/**
* @brief    Draw using OpenGL API
*
* @detail   + Bind the VAO for this GLMesh.
*           + Draw.
*           + Unbind
*
* @param    model Model matrix
* @param    view View matrix
* @param    projection Projection matrix
*/
void GLMesh::draw(void) const
{
    const Mesh::FaceList &faces = getFaces();

    glBindVertexArray(vao_handle_);
    glDrawArrays(GL_TRIANGLES, 0, 3 * faces.size());
    //glDrawArrays(GL_LINE_LOOP, 0, 3 * faces.size());
}

