/*
 * Texture.hpp
 *
 *  Created on: Sep 18, 2014
 *      Author: jusabiaga
 */

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

// Local includes
#include "gl_core_4_2.h"            // glLoadGen generated header file

// Global includes
#include <JU/core/Defs.hpp>  // Basic type typedefs
#include <string>       // std::string

class Texture
{
    public:
        Texture() : handle_(0) {}
        Texture(const char* name, const char* filename)
            : name_(name), filename_(filename), handle_(0) {}

        virtual ~Texture();

        virtual bool init(const char* name, const char* filename);
        virtual bool load();
        virtual void bind() const
        {
            glBindTexture(GL_TEXTURE_2D, handle_);
        }
        JU::uint32 getHandle() const;

    private:
        std::string name_;
        std::string filename_;
        JU::uint32  handle_;
};

#endif /* TEXTURE_HPP_ */
