/*
 * ImageHelper.hpp
 *
 *  Created on: May 28, 2014
 *      Author: aitatxo
 */

#ifndef IMAGEHELPER_HPP_
#define IMAGEHELPER_HPP_

#include <JU/core/Defs.hpp>		// JU::uint32

namespace JU
{
	void imageInvertVertically(const uint32 width, const uint32 height, const uint32 channels, uint8* const data);
}

#endif /* IMAGEHELPER_HPP_ */
