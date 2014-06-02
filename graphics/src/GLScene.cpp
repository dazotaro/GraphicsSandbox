/*
 * GLScene.cpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#include "GLScene.hpp"              // GLScene



GLScene::GLScene(int width, int height) : width_(width), height_(height)
{
}



GLScene::~GLScene()
{
    // TODO Auto-generated destructor stub
}



/**
* @brief Resize the scene
*
* @param width  Width of the window
* @param height Height of the window
*/
void GLScene::resize(int width, int height)
{
    width_ = width;
    height_ = height;
}



GLSLProgram GLScene::compileAndLinkShader(const char* vertex, const char* fragment)
{
    GLSLProgram program;

    if (!program.compileShaderFromFile(vertex, GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s", program.log().c_str());
        exit(1);
    }
    if (!program.compileShaderFromFile(fragment, GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s", program.log().c_str());
        exit(1);
    }
    if (!program.link())
    {
        printf("Shader program failed to link!\n%s", program.log().c_str());
        exit(1);
    }

    program.use();

    return program;
}



GLSLProgram GLScene::compileAndLinkShader(const char* vertex, const char* geometry,  const char* fragment)
{
    GLSLProgram program;

    if (!program.compileShaderFromFile(vertex, GLSLShader::VERTEX))
    {
        printf("Vertex shader failed to compile!\n%s", program.log().c_str());
        exit(1);
    }
    if (!program.compileShaderFromFile(geometry, GLSLShader::GEOMETRY))
    {
        printf("Fragment shader failed to compile!\n%s", program.log().c_str());
        exit(1);
    }
    if (!program.compileShaderFromFile(fragment, GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s", program.log().c_str());
        exit(1);
    }
    if (!program.link())
    {
        printf("Shader program failed to link!\n%s", program.log().c_str());
        exit(1);
    }

    program.use();

    return program;
}



const char* GLScene::getGLSLCurrentProgramString() const
{
    return current_program_iter_->first.c_str();
}

