/*
 * TextureManager.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "TextureManager.hpp"       // Class declaration
#include "GLSLProgram.hpp"          // GLSLProgram
#include "ImageHelper.hpp"			// imageInvertVertically
// Global includes
#include <SOIL.h>                   // SOIL_load_image
#include <iostream>                 // cout, endl

// STATIC VARIABLES
std::map<std::string, GLuint> TextureManager::texture_map_;  //!< Handle to the texture
int TextureManager::num_tex_bound_ = 0;

// FUNCTIONS

bool TextureManager::loadTexture(const std::string &texture_name, const std::string &filename)
{
    // If the texture is not yet in memory
    if (texture_map_.find(texture_name) == texture_map_.end())
    {
        GLuint tex_2d;
        glGenTextures(1, &tex_2d);
        texture_map_[texture_name] = tex_2d;
    }


    int width, height, channels;
    unsigned char *image = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

    GLuint mode;

    switch (channels)
    {
    	case 3:
			mode = GL_RGB;
			break;

		case 4:
			mode = GL_RGBA;
			break;

		default:
			std::printf("Loading \"%s\": number or channels %i not supported\n", filename.c_str(), channels);
			exit(0);
    }

    // Flip the image vertically
    JU::imageInvertVertically(width, height, channels, image);

    glBindTexture(GL_TEXTURE_2D, texture_map_[texture_name]);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, image);

    GLfloat filtering_mode = GL_NEAREST;


    //GLfloat filtering_mode = GL_LINEAR_MIPMAP_LINEAR;
    //glGenerateMipmap(GL_TEXTURE_2D);


    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering_mode);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering_mode);

    /*
    float color[] = { 1.0f, 0.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	*/

    SOIL_free_image_data(image);

    return true;
}


/*
bool TextureManager::loadTexture(const std::string &texture_name, const std::string &filename)
{
	//std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << texture_name.c_str() << ", " << filename.c_str() << std::endl;

	GLuint tex_id = SOIL_CREATE_NEW_ID;

	std::map<std::string, GLuint>::const_iterator texture_iter = texture_map_.find(texture_name);
	// If the texture is not yet in memory
    if (texture_iter != texture_map_.end())
    {
    	tex_id = texture_iter->second;
    }

	tex_id = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, tex_id, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	std::cout << "Texture id retrieved by SOIL_load_OGL_texture = " << tex_id << std::endl;

	if(tex_id == 0)
		return false;

	texture_map_[texture_name] = tex_id;

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return true;
}
*/



void TextureManager::bindTexture(const GLSLProgram &program, const std::string &texture_name, const std::string &uniform_name)
{
    glActiveTexture(GL_TEXTURE0 + num_tex_bound_);
    glBindTexture(GL_TEXTURE_2D, texture_map_[texture_name]);

    program.setUniform(uniform_name.c_str(), num_tex_bound_);

    num_tex_bound_++;
}

void TextureManager::unbindAllTextures()
{
    num_tex_bound_ = 0;
}



void TextureManager::deleteAllTextures()
{
    TextureMapIterator iter = texture_map_.begin();
    for(; iter != texture_map_.end(); ++iter)
    {
    	glDeleteTextures(1, &iter->second);
    }
}


