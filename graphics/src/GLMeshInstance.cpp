/*
 * GLMeshInstance.cpp
 *
 *  Created on: May 8, 2013
 *      Author: jusabiaga
 */

#include "GLMeshInstance.hpp"
#include "GLMesh.hpp"                       // GLMesh
#include "GLSLProgram.hpp"                  // GLSLProgram
#include <glm/gtc/matrix_transform.hpp>     // glm::scale
#include <glm/gtx/transform2.hpp>           // glm::scale
#include <iostream>                         // std::cout, std::endl
#include "DebugGlm.hpp"                     // overloaded operator<< for glm types
#include <sstream>                          // ostringstream
#include "TextureManager.hpp"               // bindTexture


/**
* @brief Non-Default constructor
*
* @param mesh Model matrix
* @param scaleX X axis scale factor
* @param scaleY Y axis scale factor
* @param scaleZ Z axis scale factor
*/
GLMeshInstance::GLMeshInstance(const GLMesh *mesh,
                               float scaleX,
                               float scaleY,
                               float scaleZ) :
        mesh_(mesh), scaleX_(scaleX), scaleY_(scaleY), scaleZ_(scaleZ)
{
}


/**
* @brief Set a color texture (there can be more than one)
*
* @param texture_name Name of texture (as stored by TextureManager)
*/
void GLMeshInstance::addColorTexture(const std::string &texture_name)
{
    color_texture_name_list_.push_back(texture_name);
}

/**
* @brief Set a "normal map" texture (there can only be one)
*
* @param texture_name Name of texture (as stored by TextureManager)
*/
void GLMeshInstance::addNormalTexture(const std::string &texture_name)
{
    normal_map_texture_name_ = texture_name;
}



/**
* @brief Destructor
*/
GLMeshInstance::~GLMeshInstance()
{
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
void GLMeshInstance::draw(const GLSLProgram &program, const glm::mat4 & model, const glm::mat4 &view, const glm::mat4 &projection) const
{
    // Update Model matrix with the local scale
    glm::mat4 new_model = model * glm::scale(glm::vec3(scaleX_, scaleY_, scaleZ_));
    // View * Model
    glm::mat4 mv = view * new_model;
    // Compute MVP matrix_transform
    glm::mat4 MVP (projection * view * new_model);

    // LOAD UNIFORMS
    program.setUniform("Model", new_model);
    program.setUniform("ModelViewMatrix", mv);
    program.setUniform("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
    program.setUniform("MVP", projection * mv);

    // Bind all COLOR TEXTURES
    for (int index = 0; index < color_texture_name_list_.size(); ++index)
    {
        std::ostringstream oss;;
        oss << GLSLProgram::COLOR_TEX_PREFIX << index;
        TextureManager::bindTexture(program, color_texture_name_list_[index], oss.str());
    }

    // Bind NORMAL TEXTURE
    TextureManager::bindTexture(program, normal_map_texture_name_,GLSLProgram::NORMAL_MAP_TEX_PREFIX);
    mesh_->draw();

    TextureManager::unbindAllTextures();

    // Deactivate Shader Program
    //glUseProgram(0);
}
