/*
 * TextureManager.hpp
 *
 *  Created on: Jul 24, 2013
 *      Author: jusabiaga
 */

#ifndef TEXTUREMANAGER_HPP_
#define TEXTUREMANAGER_HPP_

#include <GL/glew.h>
#include <string>
#include <map>

// FORWARD DECLARATIONS
class GLSLProgram;

class TextureManager
{
    public:
        static bool loadTexture(const std::string &texture_name, const std::string &filename);
        static void bindTexture(const GLSLProgram &program, const std::string &texture_name, const std::string &uniform_name);
        static void unbindAllTextures();

    private:
        static std::map<std::string, GLuint> texture_map_;  //!< Handle to the texture
        static int num_tex_bound_;                          //!< Number of textures already bound to some GL_ACTIVEX
};

#endif /* TEXTUREMANAGER_HPP_ */
