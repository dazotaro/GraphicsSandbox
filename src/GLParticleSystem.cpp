/*
 * GLParticleSystem.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: jusabiaga
 */

#include "GLParticleSystem.hpp"

GLParticleSystem::GLParticleSystem() : max_particles_(0)
{
    vao_handle_  = 0;
    vbo_handles_ = 0;
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
    max_particles_ = num_particles;

    positions_.resize(max_particles_, glm::vec3(0.0f));
    colors_.resize(max_particles_, glm::vec4(0.0f));

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
                 max_particles_ * sizeof(positions_[0]),
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
                 max_particles_ * sizeof(colors_[0]),
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



void GLParticleSystem::setParticleData(const std::vector<glm::vec3>& positions, const std::vector<glm::vec4>& colors)
{
	if (positions.size() != colors.size())
	{
		printf("Position and color vectors differ in size (%i vs %i)\n", positions.size(), colors.size());
		exit(0);
	}

	set_positions(positions);
	set_colors(colors);
}



void GLParticleSystem::set_positions(const std::vector<glm::vec3>& positions)
{
	if (positions_.size() >= max_particles_)
	{
		printf("Number of positions of particles (%i) larger than max (%i)\n", positions.size(), max_particles_);
		exit(0);
	}

	positions_ = positions;
}



void GLParticleSystem::set_colors(const std::vector<glm::vec4>& colors)
{
	if (colors_.size() >= max_particles_)
	{
		printf("Number of colors of particles (%i) larger than max (%i)\n", colors.size(), max_particles_);
		exit(0);
	}

	colors_ = colors;
}



/**
* @brief    Draw using OpenGL API
*
* @detail   It needs to:
*           + Activate the GLMeshInstance Shader Program
*           + Set the Uniform variables
*           + Deactivate the Shader Program
*
* @param model      Model matrix
* @param view       View matrix
* @param projection Projection matrix
*/
void GLParticleSystem::draw(const GLSLProgram &program, const glm::mat4 & model, const glm::mat4 &view, const glm::mat4 &projection) const
{
    // View * Model
    glm::mat4 mv = view * model;
    // Compute MVP matrix_transform
    glm::mat4 MVP (projection * view * model);

    // LOAD UNIFORMS
    program.setUniform("Model", model);
    program.setUniform("ModelViewMatrix", mv);
    program.setUniform("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
    program.setUniform("MVP", projection * mv);

    // Position VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[0]);
    glBufferSubData(GL_ARRAY_BUFFER,
    					0,
    					positions_.size() * sizeof(positions_[0]),
                 	 	&positions_[0]);

    // Color VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_handles_[1]);
    glBufferSubData(GL_ARRAY_BUFFER,
    					0,
    					colors_.size() * sizeof(colors_[0]),
                 	 	&colors_[0]);

    glBindVertexArray(vao_handle_);
    glDrawArrays(GL_POINTS, 0, positions_.size());

    // Deactivate Shader Program
    //glUseProgram(0);
}


