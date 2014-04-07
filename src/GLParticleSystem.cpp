/*
 * GLParticleSystem.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: jusabiaga
 */

#include "GLParticleSystem.hpp"

GLParticleSystem::GLParticleSystem() : num_particles_(0)
{
}



GLParticleSystem::~GLParticleSystem()
{
    // Delete the buffers
    glDeleteBuffers(2, vbo_handles_);
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
bool GLParticleSystem::init(JU::uint32 num_particles)
{
    num_particles_ = num_particles;

    positions_.resize(num_particles_, glm::vec3(0.0f));
    colors_.resize(num_particles_, glm::vec4(0.0f));

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
bool GLParticleSystem::initVBOs(void)
{
    // PARTICLES: VAO and BFO
    // ----------------------
    // VAO
    glGenVertexArrays(1, &vao_handle_);
    glBindVertexArray(vao_handle_);

    // VBO
    vbo_handles_ = new GLuint[2];
    glGenBuffers(2, vbo_handles_);

    // Position VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[0]);

    glBufferData(GL_ARRAY_BUFFER,
                 num_particles_ * sizeof(positions_[0]),
                 &positions_[0],
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,
                          POSITION_VECTOR_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (GLubyte *)NULL);

    glEnableVertexAttribArray(0);   // Vertex positions

    // Color VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[1]);

    glBufferData(GL_ARRAY_BUFFER,
                 num_particles_ * sizeof(colors_[0]),
                 &colors_[0],
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(1,
                          COLOR_VECTOR_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (GLubyte *)NULL);

    glEnableVertexAttribArray(1);   // Vertex colors

    return true;
}
