/*
 * SpriteSheet.hpp
 *
 *  Created on: Sep 18, 2014
 *      Author: jusabiaga
 */

#ifndef SPRITESHEET_HPP_
#define SPRITESHEET_HPP_

// Local includes
#include "Texture.hpp"  // Texture parent class

// Global Includes
#include <string>       // std::string
#include <JU/Defs.hpp>  // Basic data types

class SpriteSheet : public Texture
{
    public:
        SpriteSheet() : num_rows_(0), num_cols_(0) {}

        SpriteSheet(const char* name, const char* filename, JU::uint32 num_rows, JU::uint32 num_cols)
                : Texture(name, filename), num_rows_(num_rows), num_cols_(num_cols) {}

        bool init(const char* name, const char* filename, JU::uint32 num_rows, JU::uint32 num_cols);

        JU::uint32 getNumRows() const { return num_rows_; }
        JU::uint32 getNumCols() const { return num_cols_; }

        virtual ~SpriteSheet();

    private:
        JU::uint32 num_rows_;
        JU::uint32 num_cols_;
};

#endif /* SPRITESHEET_HPP_ */
