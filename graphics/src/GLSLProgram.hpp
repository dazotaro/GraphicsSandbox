#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H


// Local Includes
#include "gl_core_4_2.h"                // glLoadGen generated header file

// Global Includes
#include <string>               // std::string
#include <glm/glm.hpp>          // glm::vecX
#include <JU/Defs.hpp>          // JU::int32



namespace GLSLShader
{
    enum GLSLShaderType
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        TESS_CONTROL,
        TESS_EVALUATION
    };
};



/*
 * @brief   Class encapsulating a GLSL Program
 *
 * @detail  Class encapsulating a GLSL Program that also adds some member functions to set uniform variables. This class was mostly copied from the Cookbook code.
 */
class GLSLProgram
{
    public:

        static const std::string COLOR_TEX_PREFIX;
        static const std::string NORMAL_MAP_TEX_PREFIX;

        GLSLProgram();

        bool compileShaderFromFile( const char * fileName, GLSLShader::GLSLShaderType type );
        bool compileShaderFromString( const std::string & source, GLSLShader::GLSLShaderType type );
        bool link();
        bool validate();
        void use() const;

        std::string log() const;

        GLuint getHandle() const;
        bool isLinked() const;

        void bindAttribLocation( GLuint location, const char * name);
        void bindFragDataLocation( GLuint location, const char * name );

        void setUniform( const char *name, float x, float y, float z) const;
        void setUniform( const char *name, const glm::vec3 & v) const;
        void setUniform( const char *name, const glm::vec4 & v) const;
        void setUniform( const char *name, const glm::mat4 & m) const;
        void setUniform( const char *name, const glm::mat3 & m) const;
        void setUniform( const char *name, float val ) const;
        void setUniform( const char *name, int val ) const;
        void setUniform( const char *name, bool val ) const;

        void printActiveUniforms() const;
        void printActiveAttribs() const;

    private:
        GLint getUniformLocation(const char * name ) const;
        bool  fileExists( const std::string & fileName );

    private:
        GLuint      handle_;
        bool        linked_;
        std::string log_string_;
};

#endif // GLSLPROGRAM_H
