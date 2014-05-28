/*
 * ImageHelper.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Dazotaro
 */

// Local includes
#include "ImageHelper.hpp"

namespace JU
{

/**
* @brief Invert Image Vertically
*
* OpenGL's (0,0) texture coordinate is at the bottom left of an image, so some image file formats require flipping
* the Y axis
*
* @param width  	Width of the image
* @param height     Height of the image
* @param channels  	Number of channels of the image (usually either RGB = 3, or RGBA = 4)
* @param image      Pointer to the data
*/
void imageInvertVertically(const uint32 width, const uint32 height, const uint32 channels, uint8* const image)
{
	uint32 i, j;
	for( j = 0; j*2 < height; ++j )
	{
		uint32 index1 = j * width * channels;
		uint32 index2 = (height - 1 - j) * width * channels;
		for( i = width * channels; i > 0; --i )
		{
			uint8 temp = image[index1];
			image[index1] = image[index2];
			image[index2] = temp;
			++index1;
			++index2;
		}
	}
}

} // Namespace JU



