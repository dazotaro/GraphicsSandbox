/*
 * SpriteSheet.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: jusabiaga
 */

#include "SpriteSheet.hpp"



SpriteSheet::~SpriteSheet()
{
    // TODO Auto-generated destructor stub
}



bool SpriteSheet::init(const char* name, const char* filename, JU::uint32 num_rows, JU::uint32 num_cols)
{
    num_rows_ = num_rows;
    num_cols_ = num_cols;

    return Texture::init(name, filename);
}
