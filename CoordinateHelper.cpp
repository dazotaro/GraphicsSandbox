/*
 * CoordinateHelper.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: aitatxo
 */

// Local includes
#include "CoordinateHelper.hpp"

// Global includes
#include <cmath>	// sqrt, cos, acos, sin, atan

namespace CoordinateHelper
{

void cartesian2spherical(float x, float y, float z, float &radius, float &inclination, float &azimuth)
{
	radius 		= sqrt(x*x + y*y + z*z);
	inclination = acos(z / radius);
	if (y == 0)
		azimuth = 0.0f;;
	azimuth 	= atan(y / x);
}

void spherical2cartesian(float radius, float inclination, float azimuth, float &x, float &y, float &z)
{
    x = radius * sin(inclination) * cos(azimuth);
    y = radius * sin(inclination) * sin(azimuth);
    z = radius * cos(inclination);
}

} /* namespace CoordinateHelper */
