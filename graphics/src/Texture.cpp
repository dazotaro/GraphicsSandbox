/*
 * Texture.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: jusabiaga
 */

// Local includes
#include "Texture.hpp"

// Global includes
#include <SOIL.h>                   // SOIL_load_image
#include "ImageHelper.hpp"       	// imageInvertVertically
#include <cstdio>                   // std::printf



Texture::~Texture()
{
    // TODO Auto-generated destructor stub
}



bool Texture::init(const char* name, const char* filename)
{
    name_ = name;
    filename_ = filename;

    return load();
}


bool Texture::load()
{
    glGenTextures(1, &handle_);


    int width, height, channels;
    unsigned char *image = SOIL_load_image(filename_.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

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
            std::printf("Loading \"%s\": number or channels %i not supported\n", filename_.c_str(), channels);
            exit(0);
    }

    // Flip the image vertically
    JU::imageInvertVertically(width, height, channels, image);

    glBindTexture(GL_TEXTURE_2D, handle_);
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



JU::uint32 Texture::getHandle() const
{
    return handle_;
}


