/*
 * Particle.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: dazotaro
 */

// Local includes
#include "Particle.hpp"

namespace JU
{

/**
* Constructor
*
* @param position	Initial position
* @param velocity	Initial velocity
* @param mass		Mass of the particle
* @param lifetime	Life (in seconds) of this particle
*/
Particle::Particle(f32 position[3], f32 velocity[3], f32 mass, f32 lifetime)
					: mass_ (mass), lifetime_(lifetime)
{
	for (uint8 i = 0; i < 3; i++)
	{
		position_[i] = position[i];
		velocity_[i] = velocity[i];
		forces_[i] 	 = 0.0f;
	}
}



/**
* Destructor
*/
Particle::~Particle() {
	// TODO Auto-generated destructor stub
}

} /* namespace JU */
