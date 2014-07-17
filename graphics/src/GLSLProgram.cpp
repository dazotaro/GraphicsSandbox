

// Local includes
#include "GLSLProgram.hpp"    // GLSLProgram


// Global includes
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>


// MACROS
#define DEBUG 0


// CONST AND STATIC
#if DEBUG
    static std::ostream &debug = std::cout;
#else
    static std::ostringstream debug;
#endif // DEBUG

const std::string GLSLProgram::COLOR_TEX_PREFIX ("ColorTex");
const std::string GLSLProgram::NORMAL_MAP_TEX_PREFIX ("NormalMapTex");


GLSLProgram::GLSLProgram() : handle_(0), linked_(false) {}



bool GLSLProgram::compileShaderFromFile(const char * fileName, GLSLShader::GLSLShaderType type)
{
    if (!fileExists(fileName))
    {
        log_string_ = "File not found.";
        return false;
    }

    if (handle_ <= 0)
    {
        handle_ = glCreateProgram();
        if( handle_ == 0)
        {
            log_string_ = "Unable to create shader program.";
            return false;
        }
    }

    std::printf("Compiling file %s\n", fileName);

    std::ifstream inFile( fileName, std::ios::in );
    if (!inFile)
    {
        return false;
    }

    std::ostringstream code;
    while (inFile.good())
    {
        JU::int32 c = inFile.get();
        if (!inFile.eof())
            code << (char) c;
    }
    inFile.close();

    return compileShaderFromString(code.str(), type);
}



bool GLSLProgram::compileShaderFromString(const std::string & source, GLSLShader::GLSLShaderType type)
{
    if (handle_ <= 0)
    {
        handle_ = glCreateProgram();
        if (handle_ == 0)
        {
            log_string_ = "Unable to create shader program.";
            return false;
        }
    }

    GLuint shaderhandle_ = 0;

    switch (type)
    {
        case GLSLShader::VERTEX:
            shaderhandle_ = glCreateShader(GL_VERTEX_SHADER);
            break;
        case GLSLShader::FRAGMENT:
            shaderhandle_ = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case GLSLShader::GEOMETRY:
            shaderhandle_ = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case GLSLShader::TESS_CONTROL:
            shaderhandle_ = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case GLSLShader::TESS_EVALUATION:
            shaderhandle_ = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        default:
            return false;
    }

    const char * c_code = source.c_str();
    glShaderSource(shaderhandle_, 1, &c_code, NULL);

    // Compile the shader
    glCompileShader(shaderhandle_ );

    // Check for errors
    JU::int32 result;
    glGetShaderiv(shaderhandle_, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result)
    {
        // Compile failed, store log and return false
        JU::int32 length = 0;
        log_string_ = "";
        glGetShaderiv(shaderhandle_, GL_INFO_LOG_LENGTH, &length);
        if (length > 0)
        {
            char * c_log = new char[length];
            JU::int32 written = 0;
            glGetShaderInfoLog(shaderhandle_, length, &written, c_log);
            log_string_ = c_log;
            delete [] c_log;
        }

        return false;
    }
    else
    {
        // Compile succeeded, attach shader and return true
        glAttachShader(handle_, shaderhandle_);
        return true;
    }
}



bool GLSLProgram::link()
{
    if( linked_ ) return true;
    if( handle_ <= 0 ) return false;

    glLinkProgram(handle_);

    GLint status = 0;
    glGetProgramiv( handle_, GL_LINK_STATUS, &status);
    if (GL_FALSE == status)
    {
        // Store log and return false
        GLint length = 0;
        log_string_ = "";

        glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &length );

        if (length > 0)
        {
            char * c_log = new char[length];
            GLint written = 0;
            glGetProgramInfoLog(handle_, length, &written, c_log);
            log_string_ = c_log;
            delete [] c_log;
        }

        return false;
    }
    else
    {
        linked_ = true;
        return linked_;
    }
}



void GLSLProgram::use() const
{
    if(handle_ <= 0 || (!linked_))
        return;

    glUseProgram(handle_);
}



std::string GLSLProgram::log() const
{
    return log_string_;
}



GLuint GLSLProgram::getHandle() const
{
    return handle_;
}



bool GLSLProgram::isLinked() const
{
    return linked_;
}



void GLSLProgram::bindAttribLocation(GLuint location, const char * name)
{
    glBindAttribLocation(handle_, location, name);
}



void GLSLProgram::bindFragDataLocation(GLuint location, const char * name)
{
    glBindFragDataLocation(handle_, location, name);
}



void GLSLProgram::setUniform(const char *name, float x, float y, float z) const
{
    GLint loc = getUniformLocation(name);

    if (loc >= 0)
    {
        glUniform3f(loc,x,y,z);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}



void GLSLProgram::setUniform(const char *name, const glm::vec3 & v) const
{
    this->setUniform(name,v.x,v.y,v.z);
}



void GLSLProgram::setUniform(const char *name, const glm::vec4 & v) const
{
    GLint loc = getUniformLocation(name);

    if (loc >= 0)
    {
        glUniform4f(loc,v.x,v.y,v.z,v.w);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}



void GLSLProgram::setUniform(const char *name, const glm::mat4 & m) const
{
    GLint loc = getUniformLocation(name);

    if (loc >= 0)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}



void GLSLProgram::setUniform(const char *name, const glm::mat3 & m) const
{
    GLint loc = getUniformLocation(name);

    if (loc >= 0)
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}



void GLSLProgram::setUniform(const char *name, float val) const
{
    GLint loc = getUniformLocation(name);

    if (loc >= 0)
    {
        glUniform1f(loc, val);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}



void GLSLProgram::setUniform(const char *name, JU::int32 val) const
{
    GLint loc = getUniformLocation(name);

    if (loc >= 0)
    {
        glUniform1i(loc, val);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}



void GLSLProgram::setUniform(const char *name, bool val) const
{
    GLint loc = getUniformLocation(name);

    if (loc >= 0)
    {
        glUniform1i(loc, val);
    }
    else
    {
        debug << "Uniform: " << name << " not found." << std::endl;
    }
}



void GLSLProgram::printActiveUniforms()  const
{
    GLint nUniforms, size, location, maxLen;
    GLchar * name;
    GLsizei written;
    GLenum type;

    glGetProgramiv(handle_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv(handle_, GL_ACTIVE_UNIFORMS, &nUniforms);

    name = (GLchar *) malloc( maxLen );

    printf(" Location | Name\n");
    printf("------------------------------------------------\n");
    for (JU::int32 i = 0; i < nUniforms; ++i)
    {
        glGetActiveUniform( handle_, i, maxLen, &written, &size, &type, name );
        location = glGetUniformLocation(handle_, name);
        printf(" %-8d | %s\n",location, name);
    }

    free(name);
}



void GLSLProgram::printActiveAttribs() const
{

    GLint written, size, location, maxLength, nAttribs;
    GLenum type;
    GLchar * name;

    glGetProgramiv(handle_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    glGetProgramiv(handle_, GL_ACTIVE_ATTRIBUTES, &nAttribs);

    name = (GLchar *) malloc( maxLength );

    printf(" Index | Name\n");
    printf("------------------------------------------------\n");
    for (JU::int32 i = 0; i < nAttribs; i++)
    {
        glGetActiveAttrib( handle_, i, maxLength, &written, &size, &type, name );
        location = glGetAttribLocation(handle_, name);
        printf(" %-5d | %s\n",location, name);
    }

    free(name);
}



bool GLSLProgram::validate()
{
    if (!isLinked())
        return false;

    GLint status;
    glValidateProgram( handle_ );
    glGetProgramiv( handle_, GL_VALIDATE_STATUS, &status );

    if (GL_FALSE == status)
    {
        // Store log and return false
        int length = 0;
        log_string_ = "";

        glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &length );

        if (length > 0)
        {
            char * c_log = new char[length];
            JU::int32 written = 0;
            glGetProgramInfoLog(handle_, length, &written, c_log);
            log_string_ = c_log;
            delete [] c_log;
        }

        return false;
    }
    else
    {
       return true;
    }
}



GLint GLSLProgram::getUniformLocation(const char * name) const
{
    return glGetUniformLocation(handle_, name);
}



bool GLSLProgram::fileExists( const std::string & fileName )
{
    struct stat info;
    JU::int32 ret = -1;

    ret = stat(fileName.c_str(), &info);
    return 0 == ret;
}
