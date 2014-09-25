/*
 * TextureManager.hpp
 *
 *  Created on: Jul 24, 2013
 *      Author: jusabiaga
 */

#ifndef TEXTUREMANAGER_HPP_
#define TEXTUREMANAGER_HPP_

// Local Includes
#include "gl_core_4_2.h"                // glLoadGen generated header file

// Global Includes
#include "Defs.hpp"              // JU::uint32
#include <string>
#include <map>

// FORWARD DECLARATIONS
class GLSLProgram;

class TextureManager
{
    public:
		static bool loadTexture     (const std::string &texture_name, const std::string &filename);
		static bool registerTexture (const std::string &texture_name, JU::uint32 tex_id);
		static void bindTexture     (const std::string &texture_name);
		static void bindTexture     (const GLSLProgram &program, const std::string &texture_name, const std::string &uniform_name);
        static void bindTexture     (const GLSLProgram &program, JU::uint32 tex_id, const std::string &uniform_name);
        static void unbindAllTextures();
        static void deleteTexture   (const std::string& texture_name);
        static void deleteAllTextures();

    private:
        typedef std::map<std::string, GLuint> TextureMap;
        typedef TextureMap::iterator TextureMapIterator;
        static TextureMap texture_map_;  //!< Handle to the texture
        static int num_tex_bound_;                          //!< Number of textures already bound to some GL_ACTIVEX
};

#endif /* TEXTUREMANAGER_HPP_ */
